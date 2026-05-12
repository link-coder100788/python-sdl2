#include "RbCore.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>

static void window_mark(void* ptr) {
    RSDLWindow* self = (RSDLWindow*)ptr;
    if (self) rb_gc_mark(self->key_callbacks);
}

static void window_free(void* ptr) {
    RSDLWindow* self = (RSDLWindow*)ptr;
    if (self) {
        if (self->renderer) SDL_DestroyRenderer(self->renderer);
        if (self->window) SDL_DestroyWindow(self->window);
        xfree(self);
    }
}

static size_t window_size(const void* ptr) { return sizeof(RSDLWindow); }

static const rb_data_type_t window_type = {
    "ruby_sdl2/window", { window_mark, window_free, window_size }, 0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};

static void player_free(void* ptr) {
    ROpenALPlayer* self = (ROpenALPlayer*)ptr;
    if (self) {
        if (self->path) free(self->path);
        xfree(self);
    }
}

static size_t player_size(const void* ptr) { return sizeof(ROpenALPlayer); }

static const rb_data_type_t player_type = {
    "ruby_sdl2/openal_player", { NULL, player_free, player_size }, 0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};

static void sprite_free(void* ptr) {
    RSprite* self = (RSprite*)ptr;
    if (self) {
        if (self->surface) SDL_FreeSurface(self->surface);
        xfree(self);
    }
}

static size_t sprite_size(const void* ptr) { return sizeof(RSprite); }

static const rb_data_type_t sprite_type = {
    "ruby_sdl2/sprite", { NULL, sprite_free, sprite_size }, 0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};

static void coord_free(void* ptr) {
    RScreenCoordinate* self = (RScreenCoordinate*)ptr;
    if (self) xfree(self);
}

static size_t coord_size(const void* ptr) { return sizeof(RScreenCoordinate); }

static const rb_data_type_t coord_type = {
    "ruby_sdl2/screen_coordinate", { NULL, coord_free, coord_size }, 0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};

static void vec_free(void* ptr) {
    RVec3* self = (RVec3*)ptr;
    if (self) xfree(self);
}

static size_t vec_size(const void* ptr) { return sizeof(RVec3); }

static const rb_data_type_t vec_type = {
    "ruby_sdl2/vec3", { NULL, coord_free, coord_size }, 0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};

static void capsule_free(void* ptr) {
    SDL_Window* window = (SDL_Window*)ptr;
    if (window) SDL_DestroyWindow(window);
}

static const rb_data_type_t capsule_type = {
    "ruby_sdl2/capsule", { NULL, capsule_free, NULL }, 0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};

extern "C" VALUE window_alloc(VALUE klass) {
    RSDLWindow* self;
    VALUE obj = TypedData_Make_Struct(klass, RSDLWindow, &window_type, self);
    self->window = NULL;
    self->renderer = NULL;
    self->key_callbacks = rb_hash_new();
    return obj;
}

extern "C" VALUE window_init(VALUE rb_self, VALUE r_title, VALUE r_w, VALUE r_h) {
    RSDLWindow* self;
    TypedData_Get_Struct(rb_self, RSDLWindow, &window_type, self);
    const char* title = StringValueCStr(r_title);
    self->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, NUM2INT(r_w), NUM2INT(r_h), SDL_WINDOW_SHOWN);
    if (!self->window) ThrowSDLError("SDL_CreateWindow failed: %s", SDL_GetError());
    self->renderer = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_ACCELERATED);
    if (!self->renderer) {
        SDL_DestroyWindow(self->window);
        ThrowSDLError("SDL_CreateRenderer failed: %s", SDL_GetError());
    }
    SDL_ShowWindow(self->window);
    SDL_RaiseWindow(self->window);
    SDL_PumpEvents();
    return rb_self;
}

extern "C" VALUE window_poll_events(VALUE rb_self) {
    RSDLWindow* self;
    TypedData_Get_Struct(rb_self, RSDLWindow, &window_type, self);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            std::cout << "Closing Window" << std::endl;
            return Qfalse;
        }
        if (event.type == SDL_KEYDOWN) {
            int key = event.key.keysym.sym;
            VALUE r_key = INT2NUM(key);
            VALUE callback = rb_hash_aref(self->key_callbacks, r_key);
            if (!NIL_P(callback) && rb_respond_to(callback, rb_intern("call"))) {
                rb_funcall(callback, rb_intern("call"), 1, r_key);
            }
        }
    }
    return Qtrue;
}

extern "C" VALUE window_clear(VALUE rb_self, VALUE r, VALUE g, VALUE b, VALUE a) {
    RSDLWindow* self;
    TypedData_Get_Struct(rb_self, RSDLWindow, &window_type, self);
    SDL_SetRenderDrawColor(self->renderer, NUM2INT(r), NUM2INT(g), NUM2INT(b), NUM2INT(a));
    SDL_RenderClear(self->renderer);
    return Qnil;
}

extern "C" VALUE window_present(VALUE rb_self) {
    RSDLWindow* self;
    TypedData_Get_Struct(rb_self, RSDLWindow, &window_type, self);
    SDL_RenderPresent(self->renderer);
    return Qnil;
}

extern "C" VALUE window_draw_sprite(int argc, VALUE* argv, VALUE rb_self) {
    VALUE r_obj, r_scale, r_bbox;
    rb_scan_args(argc, argv, "12", &r_obj, &r_scale, &r_bbox);
    RSDLWindow* self;
    TypedData_Get_Struct(rb_self, RSDLWindow, &window_type, self);
    if (!rb_obj_is_kind_of(r_obj, cSprite)) rb_raise(rb_eTypeError, "Expected sprite object");
    RSprite* sprite;
    TypedData_Get_Struct(r_obj, RSprite, &sprite_type, sprite);
    int scale = NIL_P(r_scale) ? 1 : NUM2INT(r_scale);
    int boundingBox = RTEST(r_bbox) ? 1 : 0;
    const SDL_Rect dest = { sprite->x, sprite->y, sprite->width * scale, sprite->height * scale };
    SDL_Texture* texture = SDL_CreateTextureFromSurface(self->renderer, sprite->surface);
    SDL_RenderCopy(self->renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    if (boundingBox != 0) {
        SDL_SetRenderDrawColor(self->renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(self->renderer, &dest);
    }
    return Qnil;
}

extern "C" VALUE window_set_key_callback(VALUE rb_self, VALUE r_key, VALUE callback) {
    RSDLWindow* self;
    TypedData_Get_Struct(rb_self, RSDLWindow, &window_type, self);
    if (!rb_respond_to(callback, rb_intern("call"))) {
        rb_raise(rb_eRuntimeError, "Callback must be callable");
    }
    rb_hash_aset(self->key_callbacks, r_key, callback);
    return Qnil;
}

extern "C" VALUE player_alloc(VALUE klass) {
    ROpenALPlayer* self;
    VALUE obj = TypedData_Make_Struct(klass, ROpenALPlayer, &player_type, self);
    self->path = NULL;
    return obj;
}

extern "C" VALUE player_init(VALUE rb_self, VALUE r_path) {
    ROpenALPlayer* self;
    TypedData_Get_Struct(rb_self, ROpenALPlayer, &player_type, self);
    self->path = strdup(StringValueCStr(r_path));
    return rb_self;
}

extern "C" VALUE player_play_sound(VALUE rb_self) {
    ROpenALPlayer* self;
    TypedData_Get_Struct(rb_self, ROpenALPlayer, &player_type, self);
    FILE *f = fopen(self->path, "rb");
    if (!f) ThrowOpenALError("Could not open file at %s", self->path);
    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    rewind(f);
    auto *fullData = (unsigned char*)malloc(fileSize);
    fread(fullData, 1, fileSize, f);
    fclose(f);
    unsigned char *audioData = fullData + 44;
    int audioSize = (int)fileSize - 44;
    ALCdevice *device = alcOpenDevice(NULL);
    ALCcontext *context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    ALuint buffer, source;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_STEREO16, audioData, audioSize, 44100);
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, (ALint)buffer);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcef(source, AL_GAIN, 1.0f);
    alSourcePlay(source);
    printf("Playing WAV\n");
    ALint state;
    timespec ts = {0, 100000000};
    do {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        nanosleep(&ts, NULL);
    } while (state == AL_PLAYING);
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
    free(fullData);
    return Qnil;
}

extern "C" VALUE player_set_path(VALUE rb_self, VALUE r_path) {
    ROpenALPlayer* self;
    TypedData_Get_Struct(rb_self, ROpenALPlayer, &player_type, self);
    if (self->path) free(self->path);
    self->path = strdup(StringValueCStr(r_path));
    return Qnil;
}

extern "C" VALUE sprite_alloc(VALUE klass) {
    RSprite* self;
    VALUE obj = TypedData_Make_Struct(klass, RSprite, &sprite_type, self);
    self->x = 0;
    self->y = 0;
    self->surface = NULL;
    return obj;
}

extern "C" VALUE sprite_init(int argc, VALUE* argv, VALUE rb_self) {
    VALUE r_path, r_x, r_y;
    rb_scan_args(argc, argv, "12", &r_path, &r_x, &r_y);
    RSprite* self;
    TypedData_Get_Struct(rb_self, RSprite, &sprite_type, self);
    self->x = NIL_P(r_x) ? 0 : NUM2INT(r_x);
    self->y = NIL_P(r_y) ? 0 : NUM2INT(r_y);
    self->surface = SDL_LoadBMP(StringValueCStr(r_path));
    if (self->surface) {
        self->width = self->surface->w;
        self->height = self->surface->h;
    } else {
        ThrowSDLError("Failed to load BMP: %s", SDL_GetError());
    }
    return rb_self;
}

extern "C" VALUE sprite_set_location(VALUE rb_self, VALUE r_x, VALUE r_y) {
    RSprite* self;
    TypedData_Get_Struct(rb_self, RSprite, &sprite_type, self);
    self->x = NUM2INT(r_x);
    self->y = NUM2INT(r_y);
    return Qnil;
}

extern "C" VALUE sprite_set_screen_coord(VALUE rb_self, VALUE r_coord) {
    if (!rb_obj_is_kind_of(r_coord, cScreenCoordinate)) rb_raise(rb_eTypeError, "Expected screen coordinate object");
    RSprite* self;
    TypedData_Get_Struct(rb_self, RSprite, &sprite_type, self);
    RScreenCoordinate* coord;
    TypedData_Get_Struct(r_coord, RScreenCoordinate, &coord_type, coord);
    self->x = coord->x;
    self->y = coord->y;
    return Qnil;
}

extern "C" VALUE coord_alloc(VALUE klass) {
    RScreenCoordinate* self;
    return TypedData_Make_Struct(klass, RScreenCoordinate, &coord_type, self);
}

extern "C" VALUE coord_init(VALUE rb_self, VALUE r_x, VALUE r_y) {
    RScreenCoordinate* self;
    TypedData_Get_Struct(rb_self, RScreenCoordinate, &coord_type, self);
    self->x = NUM2INT(r_x);
    self->y = NUM2INT(r_y);
    return rb_self;
}

extern "C" VALUE coord_get_x(VALUE rb_self) {
    RScreenCoordinate* self;
    TypedData_Get_Struct(rb_self, RScreenCoordinate, &coord_type, self);
    return INT2NUM(self->x);
}

extern "C" VALUE coord_get_y(VALUE rb_self) {
    RScreenCoordinate* self;
    TypedData_Get_Struct(rb_self, RScreenCoordinate, &coord_type, self);
    return INT2NUM(self->y);
}

extern "C" VALUE vec_alloc(VALUE klass) {
    RVec3* self;
    return TypedData_Make_Struct(klass, RVec3, &vec_type, self);
}

extern "C" VALUE vec_init(VALUE rb_self, VALUE r_x, VALUE r_y, VALUE r_z) {
    RVec3* self;
    TypedData_Get_Struct(rb_self, RVec3, &vec_type, self);
    self->x = NUM2INT(r_x);
    self->y = NUM2INT(r_y);
    self->z = NUM2INT(r_z);
    return rb_self;
}

extern "C" VALUE mod_version(VALUE rb_self) {
    std::ostringstream ss;
    ss << "Ruby_SDL2 Version "
    << VERSION_MAJOR
    << "."
    << VERSION_MINOR
    << "."
    << VERSION_PATCH
#if __clang__
    << std::endl
    << "Compiled with Clang Version "
    << __clang_major__
    << "."
    << __clang_minor__
    << "."
    << __clang_patchlevel__
#endif
    << std::endl;
    std::string s = ss.str();
    return rb_str_new_cstr(s.c_str());
}

extern "C" VALUE mod_init_sdl2(VALUE rb_self) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) ThrowSDLError("SDL2 Init Error: %s", SDL_GetError());
    return Qnil;
}

extern "C" VALUE mod_quit_sdl2(VALUE rb_self) {
    SDL_Quit();
    return Qnil;
}

extern "C" VALUE mod_create_window(VALUE rb_self, VALUE r_title, VALUE r_w, VALUE r_h) {
    SDL_Window* window = SDL_CreateWindow(StringValueCStr(r_title), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, NUM2INT(r_w), NUM2INT(r_h), SDL_WINDOW_SHOWN);
    if (!window) ThrowSDLError("SDL_CreateWindow failed: %s", SDL_GetError());
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) ThrowSDLError("SDL_CreateRenderer failed: %s", SDL_GetError());
    SDL_RaiseWindow(window);
    SDL_ShowWindow(window);
    SDL_PumpEvents();
    return TypedData_Wrap_Struct(rb_cObject, &capsule_type, window);
}

extern "C" VALUE mod_pump_window(VALUE rb_self) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) std::cout << "Closing Window" << std::endl;
    }
    return Qnil;
}

extern "C" VALUE mod_get_ticks(VALUE rb_self) {
    return ULL2NUM(SDL_GetTicks64());
}

extern "C" VALUE mod_help(VALUE rb_self) {
    return rb_str_new2("Ruby_SDL2: Ruby Bindings for SDL2.");
}

extern "C" VALUE mod_play_sound_openal(VALUE rb_self, VALUE r_path) {
    ROpenALPlayer temp;
    temp.path = strdup(StringValueCStr(r_path));
    VALUE temp_obj = TypedData_Wrap_Struct(cOpenALPlayer, &player_type, &temp);
    player_play_sound(temp_obj);
    return Qnil;
}

extern "C" VALUE mod_debug(VALUE rb_self) {
    std::cout << "Ruby_SDL2 Debug: " << std::endl;
    return Qnil;
}

extern "C" VALUE mod_get_keycode(VALUE rb_self, VALUE r_name) {
    const char* name = StringValueCStr(r_name);
    SDL_Keycode key = SDL_GetKeyFromName(name);
    if (key == SDLK_UNKNOWN) ThrowSDLError("Could not find key: %s", name);
    return LONG2NUM(key);
}