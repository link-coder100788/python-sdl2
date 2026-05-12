#include "PyCore.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <format>

extern "C" int PySDLWindow_init(PySDLWindow* self, PyObject* args, PyObject* kwds) {
    const char* title;
    int w, h;

    if (!PyArg_ParseTuple(args, "sii", &title, &w, &h)) {
        return -1;
    }

    self->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w, h,
        SDL_WINDOW_SHOWN
    );

    if (!self->window) {
        PyErr_Format(SDLError, "SDL_CreateWindow failed: %s", SDL_GetError());
        return -1;
    }

    self->renderer = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_ACCELERATED);
    if (!self->renderer) {
        SDL_DestroyWindow(self->window);
        PyErr_Format(SDLError, "SDL_CreateRenderer failed: %s", SDL_GetError());
        return -1;
    }

    self->key_callbacks = PyDict_New();
    if (!self->key_callbacks) return -1;

    SDL_ShowWindow(self->window);
    SDL_RaiseWindow(self->window);
    SDL_PumpEvents();
    return 0;
}

extern "C" void PySDLWindow_dealloc(PySDLWindow* self) {
    if (self->renderer) SDL_DestroyRenderer(self->renderer);
    if (self->window) SDL_DestroyWindow(self->window);
    if (self->key_callbacks) Py_XDECREF(self->key_callbacks);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

extern "C" PyObject* PySDLWindow_poll_events(PySDLWindow* self, PyObject* Py_UNUSED(ignored)) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            std::cout << "Closing Window" << std::endl;
            Py_RETURN_FALSE;
        }

        if (event.type == SDL_KEYDOWN) {
            int key = event.key.keysym.sym;
            PyObject* keyObj = PyLong_FromLong(key);
            PyObject* callback = PyDict_GetItem(self->key_callbacks, keyObj);
            Py_DECREF(keyObj);
            if (callback && PyCallable_Check(callback)) {
                PyGILState_STATE gstate = PyGILState_Ensure();
                PyObject* arg = Py_BuildValue("(i)", key);
                PyObject* result = PyObject_CallObject(callback, arg);
                Py_DECREF(arg);
                if (!result) {
                    PyErr_Print();
                } else {
                    Py_DECREF(result);
                }
                PyGILState_Release(gstate);
            }
        }
    }
    Py_RETURN_TRUE;
}

extern "C" PyObject* PySDLWindow_clear(PySDLWindow* self, PyObject* args) {
    int r, g, b, a;

    if (!PyArg_ParseTuple(args, "iiii", &r, &g, &b, &a)) {
        return NULL;
    }

    SDL_SetRenderDrawColor(self->renderer, r, g, b, a);
    SDL_RenderClear(self->renderer);
    Py_RETURN_NONE;
}

extern "C" PyObject* PySDLWindow_present(PySDLWindow* self, PyObject* Py_UNUSED(ignored)) {
    SDL_RenderPresent(self->renderer);
    Py_RETURN_NONE;
}

extern "C" PyObject* PySDLWindow_draw_sprite(PySDLWindow* self, PyObject* args) {
    PyObject* obj;
    int scale = 1;
    int boundingBox = 0;
    if (!PyArg_ParseTuple(args, "O|ip", &obj, &scale, &boundingBox)) ThrowArgError("Could not parse args");
    if (!PyObject_TypeCheck(obj, &PySpriteType)) return PyErr_Format(PyExc_TypeError, "Expected sprite object");
    auto* sprite = (PySprite*)obj;
    const SDL_Rect dest = { sprite->x, sprite->y, sprite->sprite.width * scale, sprite->sprite.height * scale };
    SDL_Texture* texture = SDL_CreateTextureFromSurface(self->renderer, sprite->sprite.surface);
    SDL_RenderCopy(self->renderer, texture, NULL, &dest);
    if (boundingBox != 0) {
        SDL_SetRenderDrawColor(self->renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(self->renderer, &dest);
    }
    Py_RETURN_NONE;
}

extern "C" PyObject* PySDLWindow_set_key_callback(PySDLWindow* self, PyObject* args) {
    int key;
    PyObject* callback;
    if (!PyArg_ParseTuple(args, "iO", &key, &callback)) ThrowArgError("Couldn't parse callback");
    if (!PyCallable_Check(callback)) return PyErr_Format(PyExc_RuntimeError, "Callback must be callable");
    PyObject* keyObj = PyLong_FromLong(key);
    if (PyDict_SetItem(self->key_callbacks, keyObj, callback) < 0) {
        Py_DECREF(keyObj);
        return NULL;
    }
    Py_DECREF(keyObj);
    Py_RETURN_NONE;
}

extern "C" int PyOpenALPlayer_init(PyOpenALPlayer* self, PyObject* args, PyObject* kwds) {
    char* path;
    if (!PyArg_ParseTuple(args, "s", &path)) return -1;
    self->path = path;
    return 0;
}

extern "C" void PyOpenALPlayer_dealloc(PyOpenALPlayer* self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

extern "C" PyObject* PyOpenALPlayer_play_sound(PyOpenALPlayer* self, PyObject* args) {
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
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_STEREO16, audioData, audioSize, 44100);
    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, (ALint)buffer);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcef(source, AL_GAIN, 1.0f);
    alSourcePlay(source);
    printf("Playing WAV\n");
    ALint state;
    timespec ts {};
    ts.tv_nsec = 100000000;
    ts.tv_sec = 0;
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
    Py_RETURN_NONE;
}

extern "C" PyObject* PyOpenALPlayer_set_path(PyOpenALPlayer* self, PyObject* args) {
    char* path;
    if (!PyArg_ParseTuple(args, "s", &path)) ThrowArgError("Couldn't parse filepath");
    self->path = path;
    Py_RETURN_NONE;
}

extern "C" int PySprite_init(PySprite* self, PyObject* args, PyObject* kwds) {
    char* path;
    int x = 0;
    int y = 0;
    if (!PyArg_ParseTuple(args, "s|ii", &path, &x, &y)) return -1;
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (surface) {
        self->x = x;
        self->y = y;
        self->sprite.surface = surface;
        self->sprite.width = surface->w;
        self->sprite.height = surface->h;
        return 0;
    }
    return -1;
}

extern "C" void PySprite_dealloc(PySprite* self) {
    if (self->sprite.surface) SDL_FreeSurface(self->sprite.surface);
}

extern "C" PyObject* PySprite_set_location(PySprite* self, PyObject* args) {
    int x, y;
    if (!PyArg_ParseTuple(args, "ii", &x, &y)) ThrowArgError("Could not parse location args");
    self->x = x;
    self->y = y;
    Py_RETURN_NONE;
}

extern "C" PyObject* PySprite_set_screen_coordinate(PySprite* self, PyObject* args) {
    PyObject* obj;
    if (!PyArg_ParseTuple(args, "O", &obj)) ThrowArgError("Could not parse screen coordinate");
    if (!PyObject_TypeCheck(obj, &PyScreenCoordinateType)) return PyErr_Format(PyExc_TypeError, "Expected screen coordinate object");
    auto* screenCoord = (PyScreenCoordinate*)obj;
    if (!screenCoord) return PyErr_Format(PyExc_TypeError, "Expected screen coordinate object");
    self->x = screenCoord->x;
    self->y = screenCoord->y;
    Py_RETURN_NONE;
}

extern "C" int PyScreenCoordinate_init(PyScreenCoordinate* self, PyObject* args, PyObject* kwds) {
    int x, y;
    if (!PyArg_ParseTuple(args, "ii", &x, &y)) return -1;
    self->x = x;
    self->y = y;
    return 0;
}

extern "C" void PyScreenCoordinate_dealloc(PyScreenCoordinate* self) {

}

extern "C" int PyVec3_init(PyVec3* self, PyObject* args, PyObject* kwds) {
    self->x = 0.0f;
    self->y = 0.0f;
    self->z = 0.0f;
    if (!self) return -1;
    return 0;
}

extern "C" void PyVec3_dealloc(PyVec3* self) {

}

extern "C" void py_window_destructor(PyObject* capsule) {
    auto* window = (SDL_Window*)PyCapsule_GetPointer(capsule, "SDL_Window");
    if (window) SDL_DestroyWindow(window);
}

extern "C" PyObject* py_print_version(PyObject* self, PyObject* args) {
    SDL_version version;
    SDL_VERSION(&version)
    std::ostringstream ss;
    ss << "Python_SDL2 Version "
    << VERSION_MAJOR
    << "."
    << VERSION_MINOR
    << "."
    << VERSION_PATCH
    << std::endl
    << "SDL2 Version "
    << version.major
    << "."
    << version.minor
    << "."
    << version.patch
#if __clang__
    << "Built with Clang Version "
    << __clang_major__
    << "."
    << __clang_minor__
    << "."
    << __clang_patchlevel__
    << " "
    << __VERSION__
#endif
    << std::endl;
    return PyUnicode_FromString(ss.str().c_str());
}

extern "C" PyObject* py_init_sdl2(PyObject* self, PyObject* args) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        ThrowSDLError("SDL2 Init Error: %s", SDL_GetError());
    }
    Py_RETURN_NONE;
}

extern "C" PyObject* py_quit_sdl2(PyObject* self, PyObject* args) {
    SDL_Quit();
    Py_RETURN_NONE;
}

extern "C" PyObject* py_create_window_sdl2(PyObject* self, PyObject* args) {
    const char* title;
    int w, h;

    if (!PyArg_ParseTuple(args, "sii", &title, &w, &h)) {
        return NULL;
    }

    SDL_Window* window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        ThrowSDLError("SDL_CreateWindow failed: %s", SDL_GetError());
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        ThrowSDLError("SDL_CreateRenderer failed: %s", SDL_GetError());
    }

    SDL_RaiseWindow(window);
    SDL_ShowWindow(window);
    SDL_PumpEvents();

    return PyCapsule_New(window, "SDL_Window", py_window_destructor);
}

extern "C" PyObject* py_pump_window_sdl2(PyObject* self, PyObject* args) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            std::cout << "Closing Window" << std::endl;
        }
    }

    Py_RETURN_NONE;
}

extern "C" PyObject* py_get_ticks_now(PyObject* self, PyObject* args) {
    return PyLong_FromUInt64(SDL_GetTicks64());
}

extern "C" PyObject* py_help(PyObject* self, PyObject* args) {
    return PyUnicode_FromString("Python_SDL2: Python Bindings for SDL2. Run version() to get current version. Please file bug reports on Github");
}

extern "C" PyObject* py_playsound_openal(PyObject* self, PyObject* args) {
    char* path;
    if (!PyArg_ParseTuple(args, "s", &path)) ThrowArgError("Invalid Args");
    FILE *f = fopen(path, "rb");
    if (!f) ThrowOpenALError("Could not open file at %s", path);
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
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_STEREO16, audioData, audioSize, 44100);
    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, (ALint)buffer);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcef(source, AL_GAIN, 1.0f);
    alSourcePlay(source);
    printf("Playing WAV\n");
    ALint state;
    timespec ts {};
    ts.tv_nsec = 100000000;
    ts.tv_sec = 0;
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
    Py_RETURN_NONE;
}

extern "C" PyObject* py_debug(PyObject* self, PyObject* args) {
    std::cout << "Python_SDL2 Debug: " << std::endl;
    Py_RETURN_NONE;
}

extern "C" PyObject* py_get_keycode_from_name(PyObject* self, PyObject* args) {
    char* name;
    if (!PyArg_ParseTuple(args, "s", &name)) ThrowArgError("Could not parse args");
    SDL_Keycode key = SDL_GetKeyFromName(name);
    if (key == SDLK_UNKNOWN) ThrowSDLError("Could not find key: %s", name);
    return PyLong_FromLong(key);
}
