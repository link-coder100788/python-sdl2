#ifndef RUBY_SDL2_H
#define RUBY_SDL2_H

#include <ruby.h>
#include <SDL2/SDL.h>
#ifdef USING_OPENAL_SOFT
#include <AL/alc.h>
#include <AL/al.h>
#else
#include <OpenAL.h>
#endif

#ifndef VERSION_MAJOR
#define VERSION_MAJOR 1
#endif
#ifndef VERSION_MINOR
#define VERSION_MINOR 2
#endif
#ifndef VERSION_PATCH
#define VERSION_PATCH 0
#endif

extern VALUE mRubySDL2;
extern VALUE cWindow, cOpenALPlayer, cSprite, cScreenCoordinate, cVec3;
extern VALUE eSDLError, eOpenALError, eArgError;

#define ThrowSDLError(fmt, ...) rb_raise(eSDLError, fmt, ##__VA_ARGS__)
#define ThrowOpenALError(fmt, ...) rb_raise(eOpenALError, fmt, ##__VA_ARGS__)
#define ThrowArgError(fmt, ...) rb_raise(eArgError, fmt, ##__VA_ARGS__)

struct RSDLWindow {
    SDL_Window* window;
    SDL_Renderer* renderer;
    VALUE key_callbacks;
};

struct ROpenALPlayer {
    char* path;
};

struct RSprite {
    int x, y;
    SDL_Surface* surface;
    int width, height;
};

struct RScreenCoordinate {
    int x, y;
};

struct RVec3 {
    float x, y, z;
};

#ifdef __cplusplus
extern "C" {
#endif

VALUE window_alloc(VALUE klass);

VALUE window_init(VALUE rb_self, VALUE r_title, VALUE r_w, VALUE r_h);

VALUE window_poll_events(VALUE rb_self);

VALUE window_clear(VALUE rb_self, VALUE r, VALUE g, VALUE b, VALUE a);

VALUE window_present(VALUE rb_self);

VALUE window_draw_sprite(int argc, VALUE* argv, VALUE rb_self);

VALUE window_set_key_callback(VALUE rb_self, VALUE r_key, VALUE callback);

VALUE player_alloc(VALUE klass);

VALUE player_init(VALUE rb_self, VALUE r_path);

VALUE player_play_sound(VALUE rb_self);

VALUE player_set_path(VALUE rb_self, VALUE r_path);

VALUE sprite_alloc(VALUE klass);

VALUE sprite_init(int argc, VALUE* argv, VALUE rb_self);

VALUE sprite_set_location(VALUE rb_self, VALUE r_x, VALUE r_y);

VALUE sprite_set_screen_coord(VALUE rb_self, VALUE r_coord);

VALUE coord_alloc(VALUE klass);

VALUE coord_init(VALUE rb_self, VALUE r_x, VALUE r_y);

VALUE coord_get_x(VALUE rb_self);

VALUE coord_get_y(VALUE rb_self);

VALUE vec_alloc(VALUE klass);

VALUE vec_init(VALUE rb_self, VALUE r_x, VALUE r_y, VALUE r_z);

VALUE mod_version(VALUE rb_self);

VALUE mod_init_sdl2(VALUE rb_self);

VALUE mod_quit_sdl2(VALUE rb_self);

VALUE mod_create_window(VALUE rb_self, VALUE r_title, VALUE r_w, VALUE r_h);

VALUE mod_pump_window(VALUE rb_self);

VALUE mod_get_ticks(VALUE rb_self);

VALUE mod_help(VALUE rb_self);

VALUE mod_play_sound_openal(VALUE rb_self, VALUE r_path);

VALUE mod_debug(VALUE rb_self);

VALUE mod_get_keycode(VALUE rb_self, VALUE r_name);

#ifdef __cplusplus
}
#endif

#endif // RUBY_SDL2_H
