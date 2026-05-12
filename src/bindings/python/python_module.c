#include "../../core/PyCore.h"
#include <Python.h>
#include "generated_reflect.h"

PyObject* SDLError;
PyObject* OpenALError;
PyObject* ArgError;

static PyMethodDef PySDLWindow_methods[] = {
    { "poll_events", (PyCFunction)PySDLWindow_poll_events, METH_NOARGS, "Poll events" },
    { "clear", (PyCFunction)PySDLWindow_clear, METH_VARARGS, "Clear Screen" },
    { "present", (PyCFunction)PySDLWindow_present, METH_NOARGS, "Present renderer" },
    { "draw_sprite", (PyCFunction)PySDLWindow_draw_sprite, METH_VARARGS, "Draw a Sprite object to the screen" },
    { "set_key_callback", (PyCFunction)PySDLWindow_set_key_callback, METH_VARARGS, "Register a key callback" },
    { NULL }
};

PyTypeObject PySDLWindowType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "python_sdl2.Window",
    .tp_basicsize = sizeof(PySDLWindow),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PySDLWindow_init,
    .tp_dealloc = (destructor)PySDLWindow_dealloc,
    .tp_methods = PySDLWindow_methods,
};

static PyMethodDef PyOpenALPlayer_methods[] = {
    { "play_sound", (PyCFunction)PyOpenALPlayer_play_sound, METH_NOARGS, "Play sound at path" },
    { "set_path", (PyCFunction)PyOpenALPlayer_set_path, METH_VARARGS, "Set path" },
    { NULL }
};

PyTypeObject PyOpenALPlayerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "python_sdl2.OpenALPlayer",
    .tp_basicsize = sizeof(PyOpenALPlayer),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PyOpenALPlayer_init,
    .tp_dealloc = (destructor)PyOpenALPlayer_dealloc,
    .tp_methods = PyOpenALPlayer_methods,
};

static PyMethodDef PySprite_methods[] = {
    { "set_location", (PyCFunction)PySprite_set_location, METH_VARARGS, "Set the x and y location of the sprite" },
    { NULL }
};

PyTypeObject PySpriteType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "python_sdl2.Sprite",
    .tp_basicsize = sizeof(PySprite),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PySprite_init,
    .tp_dealloc = (destructor)PySprite_dealloc,
    .tp_methods = PySprite_methods,
};

static PyMethodDef PyScreenCoordinate_methods[] = {
    PyScreenCoordinate_REFLECTED_METHODS
    { NULL }
};

PyTypeObject PyScreenCoordinateType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "python_sdl2.ScreenCoordinate",
    .tp_basicsize = sizeof(PyScreenCoordinate),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PyScreenCoordinate_init,
    .tp_dealloc = (destructor)PyScreenCoordinate_dealloc,
    .tp_methods = PyScreenCoordinate_methods,
};

static PyMethodDef PyVec3_methods[] = {
    PyVec3_REFLECTED_METHODS
    { NULL }
};

PyTypeObject PyVec3Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "python_sdl2.Vec3",
    .tp_basicsize = sizeof(PyVec3),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PyVec3_init,
    .tp_dealloc = (destructor)PyVec3_dealloc,
    .tp_methods = PyVec3_methods,
};

static PyMethodDef methods[] = {
    { "version", py_print_version, METH_NOARGS, "Return a hello string" },
    { "init", py_init_sdl2, METH_NOARGS, "Init the SDL2 Backend" },
    { "quit", py_quit_sdl2, METH_NOARGS, "Quit the SDL2 Backend" },
    { "create_window", py_create_window_sdl2, METH_VARARGS, "Create a SDL2 Window" },
    { "pump_window", py_pump_window_sdl2, METH_NOARGS, "Keep an SDL_Window alive" },
    { "get_ticks", py_get_ticks_now, METH_NOARGS, "Get current ticks (ms) since program start" },
    { "help", py_help, METH_NOARGS, "Get Help" },
    { "play_sound", py_playsound_openal, METH_VARARGS, "Play a .wav file through OpenAL" },
    { "_debug", py_debug, METH_NOARGS, "Print Debug info" },
    { "get_keycode_from_name", py_get_keycode_from_name, METH_VARARGS, "Get keycode value from key name" },
    { NULL, NULL, 0, NULL },
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "python_sdl2",
    NULL,
    -1,
    methods
};

PyMODINIT_FUNC PyInit_python_sdl2(void) {
    PyObject* m = PyModule_Create(&module);
    if (!m) return NULL;

    SDLError = PyErr_NewException("python_sdl2.SDLError", PyExc_RuntimeError, NULL);
    if (!SDLError) return NULL;
    Py_INCREF(SDLError);
    PyModule_AddObject(m, "SDLError", SDLError);

    OpenALError = PyErr_NewException("python_sdl2.OpenALError", PyExc_RuntimeError, NULL);
    if (!OpenALError) return NULL;
    Py_INCREF(OpenALError);
    PyModule_AddObject(m, "OpenALError", OpenALError);

    ArgError = PyErr_NewException("python_sdl2.ArgumentError", PyExc_RuntimeError, NULL);
    if (!ArgError) return NULL;
    Py_INCREF(ArgError);
    PyModule_AddObject(m, "ArgumentError", ArgError);

    if (PyType_Ready(&PySDLWindowType) < 0) return NULL;
    Py_INCREF(&PySDLWindowType);
    PyModule_AddObject(m, "Window", (PyObject*)&PySDLWindowType);

    if (PyType_Ready(&PyOpenALPlayerType) < 0) return NULL;
    Py_INCREF(&PyOpenALPlayerType);
    PyModule_AddObject(m, "OpenALPlayer", (PyObject*)&PyOpenALPlayerType);

    if (PyType_Ready(&PySpriteType) < 0) return NULL;
    Py_INCREF(&PySpriteType);
    PyModule_AddObject(m, "Sprite", (PyObject*)&PySpriteType);

    if (PyType_Ready(&PyScreenCoordinateType) < 0) return NULL;
    Py_INCREF(&PyScreenCoordinateType);
    PyModule_AddObject(m, "ScreenCoordinate", (PyObject*)&PyScreenCoordinateType);

    if (PyType_Ready(&PyVec3Type) < 0) return NULL;
    Py_INCREF(&PyVec3Type);
    PyModule_AddObject(m, "Vec3", (PyObject*)&PyVec3Type);

    return m;
}