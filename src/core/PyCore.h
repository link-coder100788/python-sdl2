#ifndef PYTHON_SDL2_WINDOW_HPP
#define PYTHON_SDL2_WINDOW_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>
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

// Errors
extern PyObject* SDLError;
extern PyObject* OpenALError;
extern PyObject* ArgError;

#define ThrowSDLError(fmt, ...) return PyErr_Format(SDLError, fmt, ##__VA_ARGS__)
#define ThrowOpenALError(fmt, ...) return PyErr_Format(OpenALError, fmt, ##__VA_ARGS__)
#define ThrowArgError(fmt, ...) return PyErr_Format(ArgError, fmt, ##__VA_ARGS__)

typedef struct {
    PyObject_HEAD
    SDL_Window* window;
    SDL_Renderer* renderer;
    PyObject* key_callbacks;
} PySDLWindow;

extern PyTypeObject PySDLWindowType;

int PySDLWindow_init(PySDLWindow* self, PyObject* args, PyObject* kwds);

void PySDLWindow_dealloc(PySDLWindow* self);

PyObject* PySDLWindow_poll_events(PySDLWindow* self, PyObject* Py_UNUSED(ignored));

PyObject* PySDLWindow_clear(PySDLWindow* self, PyObject* args);

PyObject* PySDLWindow_present(PySDLWindow* self, PyObject* Py_UNUSED(ignored));

PyObject* PySDLWindow_draw_sprite(PySDLWindow* self, PyObject* args);

PyObject* PySDLWindow_set_key_callback(PySDLWindow* self, PyObject* args);

typedef struct {
    PyObject_HEAD;
    char* path;
} PyOpenALPlayer;

extern PyTypeObject PyOpenALPlayerType;

int PyOpenALPlayer_init(PyOpenALPlayer* self, PyObject* args, PyObject* kwds);

void PyOpenALPlayer_dealloc(PyOpenALPlayer* self);

PyObject* PyOpenALPlayer_play_sound(PyOpenALPlayer* self, PyObject* args);

PyObject* PyOpenALPlayer_set_path(PyOpenALPlayer* self, PyObject* args);

typedef struct {
    PyObject_HEAD;
    int x, y;
    struct {
        SDL_Surface* surface;
        int width, height;
    } sprite;
} PySprite;

extern PyTypeObject PySpriteType;

int PySprite_init(PySprite* self, PyObject* args, PyObject* kwds);

void PySprite_dealloc(PySprite* self);

PyObject* PySprite_set_location(PySprite* self, PyObject* args);

PyObject* PySprite_set_screen_coordinate(PySprite* self, PyObject* args);

typedef struct [[clang::annotate("reflected")]] PyScreenCoordinate {
    PyObject_HEAD;
    int x [[clang::annotate("reflected")]];
    int y [[clang::annotate("reflected")]];
} PyScreenCoordinate;

extern PyTypeObject PyScreenCoordinateType;

int PyScreenCoordinate_init(PyScreenCoordinate* self, PyObject* args, PyObject* kwds);

void PyScreenCoordinate_dealloc(PyScreenCoordinate* self);

typedef struct [[clang::annotate("reflected")]] PyVec3 {
    PyObject_HEAD;
    float x [[clang::annotate("reflected")]];
    float y [[clang::annotate("reflected")]];
    float z [[clang::annotate("reflected")]];
} PyVec3;

extern PyTypeObject PyVec3Type;

int PyVec3_init(PyVec3* self, PyObject* args, PyObject* kwds);

void PyVec3_dealloc(PyVec3* self);

void py_window_destructor(PyObject* capsule);

PyObject* py_print_version(PyObject* self, PyObject* args);

PyObject* py_init_sdl2(PyObject* self, PyObject* args);

PyObject* py_quit_sdl2(PyObject* self, PyObject* args);

PyObject* py_create_window_sdl2(PyObject* self, PyObject* args);

PyObject* py_pump_window_sdl2(PyObject* self, PyObject* args);

PyObject* py_get_ticks_now(PyObject* self, PyObject* args);

PyObject* py_help(PyObject* self, PyObject* args);

PyObject* py_playsound_openal(PyObject* self, PyObject* args);

PyObject* py_debug(PyObject* self, PyObject* args);

PyObject* py_get_keycode_from_name(PyObject* self, PyObject* args);

#ifdef __cplusplus
}
#endif

#endif //PYTHON_SDL2_WINDOW_HPP
