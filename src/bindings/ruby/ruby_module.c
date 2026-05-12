#include "RbCore.h"

VALUE mRubySDL2;
VALUE cWindow, cOpenALPlayer, cSprite, cScreenCoordinate, cVec3;
VALUE eSDLError, eOpenALError, eArgError;

void Init_ruby_sdl2(void) {
    mRubySDL2 = rb_define_module("RubySDL2");

    eSDLError = rb_define_class_under(mRubySDL2, "SDLError", rb_eRuntimeError);
    eOpenALError = rb_define_class_under(mRubySDL2, "OpenALError", rb_eRuntimeError);
    eArgError = rb_define_class_under(mRubySDL2, "ArgumentError", rb_eArgError);

    cWindow = rb_define_class_under(mRubySDL2, "Window", rb_cObject);
    rb_define_alloc_func(cWindow, window_alloc);
    rb_define_method(cWindow, "initialize", (VALUE(*)(VALUE, VALUE, VALUE, VALUE))window_init, 3);
    rb_define_method(cWindow, "poll_events", (VALUE(*)(VALUE))window_poll_events, 0);
    rb_define_method(cWindow, "clear", (VALUE(*)(VALUE, VALUE, VALUE, VALUE, VALUE))window_clear, 4);
    rb_define_method(cWindow, "present", (VALUE(*)(VALUE))window_present, 0);
    rb_define_method(cWindow, "draw_sprite", (VALUE(*)(int, VALUE*, VALUE))window_draw_sprite, -1);
    rb_define_method(cWindow, "set_key_callback", (VALUE(*)(VALUE, VALUE, VALUE))window_set_key_callback, 2);

    cOpenALPlayer = rb_define_class_under(mRubySDL2, "OpenALPlayer", rb_cObject);
    rb_define_alloc_func(cOpenALPlayer, player_alloc);
    rb_define_method(cOpenALPlayer, "initialize", (VALUE(*)(VALUE, VALUE))player_init, 1);
    rb_define_method(cOpenALPlayer, "play_sound", (VALUE(*)(VALUE))player_play_sound, 0);
    rb_define_method(cOpenALPlayer, "set_path", (VALUE(*)(VALUE, VALUE))player_set_path, 1);

    cSprite = rb_define_class_under(mRubySDL2, "Sprite", rb_cObject);
    rb_define_alloc_func(cSprite, sprite_alloc);
    rb_define_method(cSprite, "initialize", (VALUE(*)(int, VALUE*, VALUE))sprite_init, -1);
    rb_define_method(cSprite, "set_location", (VALUE(*)(VALUE, VALUE, VALUE))sprite_set_location, 2);
    rb_define_method(cSprite, "set_screen_coordinate", (VALUE(*)(VALUE, VALUE))sprite_set_screen_coord, 1);

    cScreenCoordinate = rb_define_class_under(mRubySDL2, "ScreenCoordinate", rb_cObject);
    rb_define_alloc_func(cScreenCoordinate, coord_alloc);
    rb_define_method(cScreenCoordinate, "initialize", (VALUE(*)(VALUE, VALUE, VALUE))coord_init, 2);
    rb_define_method(cScreenCoordinate, "x", (VALUE(*)(VALUE))coord_get_x, 0);
    rb_define_method(cScreenCoordinate, "y", (VALUE(*)(VALUE))coord_get_y, 0);
    rb_define_method(cScreenCoordinate, "get_x", (VALUE(*)(VALUE))coord_get_x, 0);
    rb_define_method(cScreenCoordinate, "get_y", (VALUE(*)(VALUE))coord_get_y, 0);

    cVec3 = rb_define_class_under(mRubySDL2, "Vec3", rb_cObject);
    rb_define_alloc_func(cVec3, vec_alloc);
    rb_define_method(cVec3, "initialize", (VALUE(*)(VALUE, VALUE, VALUE, VALUE))vec_init, 3);

    rb_define_module_function(mRubySDL2, "version", (VALUE(*)(VALUE))mod_version, 0);
    rb_define_module_function(mRubySDL2, "init", (VALUE(*)(VALUE))mod_init_sdl2, 0);
    rb_define_module_function(mRubySDL2, "quit", (VALUE(*)(VALUE))mod_quit_sdl2, 0);
    rb_define_module_function(mRubySDL2, "get_ticks", (VALUE(*)(VALUE))mod_get_ticks, 0);

    rb_define_module_function(mRubySDL2, "help", (VALUE(*)(VALUE))mod_help, 0);
    rb_define_module_function(mRubySDL2, "_debug", (VALUE(*)(VALUE))mod_debug, 0);
    rb_define_module_function(mRubySDL2, "get_keycode_from_name", (VALUE(*)(VALUE, VALUE))mod_get_keycode, 1);
    rb_define_module_function(mRubySDL2, "play_sound", (VALUE(*)(VALUE, VALUE))mod_play_sound_openal, 1);
}