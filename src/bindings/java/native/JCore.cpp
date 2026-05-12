#include <sstream>
#include <string>
#include <iostream>
#include "JCore.h"
#include <SDL2/SDL.h>
#ifdef USING_OPENAL_SOFT
#include <AL/alc.h>
#include <AL/al.h>
#else
#include <OpenAL.h>
#endif
#include <ctime>
#include <jni.h>


/**
 * @brief A namespace containing important types
 * and functions for JavaSDL2
 */
namespace jsdl2 {
    /**
     * An equivalent to a std::uintptr_t,
     * used to cast too and from malloc'd
     * types represented by a jlong in
     * supported java classes provided by
     * JavaSDL2. When used over simple long
     * casting, it prevents an issue on Windows
     * and/or (LLP64) systems where pointers
     * (64-bit) cast to long (32-bit) would
     * truncate the pointer.
     *
     * @brief An equivalent to uintptr_t
     */
    typedef uintptr_t jhandle;

    /**
     * @tparam T The type to cast back into a pointer
     * @param ptr The jhandle or the pointer of the malloc'd T
     * @return A casted T*
     */
    template <typename T>
    inline T* cast_from_jhandle(jhandle ptr) {
        return (T*)ptr;
    }

    /**
     * @tparam T The type of the pointer to cast into a jhandle
     * @param ptr The T* of the pointer to cast
     * @return A jhandle that is a cast pointer
     */
    template <typename T>
    inline jhandle cast_to_jhandle(T* ptr) {
        return (jhandle)ptr;
    }

    template <typename T, typename V>
    inline void set_field_jhandle(jhandle ptr, V T::* field, V value) {
        T* obj = cast_from_jhandle<T>(ptr);
        if (!obj) return;
        obj->*field = value;
    }

    template <typename T, typename V>
    inline V get_field_jhandle(jhandle ptr, V T::* field, V default_value) {
        T* obj = cast_from_jhandle<T>(ptr);
        if (!obj) return default_value;
        return obj->*field;
    }
}

SDL_Renderer* g_renderer = nullptr;
JavaVM* g_jvm = nullptr;

namespace OpenALSettings {
    constexpr int SAMPLE_RATE_hz = 44'100;
    constexpr int LOOP_DELAY_ns = 75'000'000; // 75 ms
}

namespace Info {
    class JClassInfoHelper {
    public:
        static void GenericInfo(const std::string& classDesc) {
            JNIEnv* env;
            g_jvm->AttachCurrentThread((void**)&env, nullptr);
            jclass c = env->FindClass(classDesc.c_str());
            if (!c) { std::cout << "Could not find class " << classDesc << std::endl; return; }
            if (env->ExceptionCheck()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }
            jmethodID mID = env->GetStaticMethodID(c, "_$dGenericInfo", "()V");
            if (!mID) { std::cout << "Could not find method _$dGenericInfo in class " << classDesc << std::endl; return; }
            if (env->ExceptionCheck()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }
            env->CallStaticVoidMethod(c, mID);
            if (env->ExceptionCheck()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }
        }

        static void GenericInfoName(const std::string& className) {
            const std::string classDesc = "dev/linkcoder100788/java_sdl2/JavaSDL2$" + className;
            GenericInfo(classDesc);
        }
    };
}

template <typename T>
inline T* cast_from_java_handle(long ptr) {
    return (T*)ptr;
}

template <typename T>
inline long cast_to_java_handle(T* ptr) {
    return (long)ptr;
}

template <bool scaled, bool bounded>
inline void draw_sprite(SDL_Renderer* r, Sprite* s, int scale = 1) {
    if (!r) return;
    if (!s) return;
    if (!s->sprite.surface) return;

    SDL_Rect dest = { s->x, s->y, s->sprite.width * (scaled ? scale : 1), s->sprite.height * (scaled ? scale : 1) };
    SDL_Texture* texture = SDL_CreateTextureFromSurface(r, s->sprite.surface);
    SDL_RenderCopy(r, texture, NULL, &dest);
    SDL_DestroyTexture(texture);

    if (bounded) {
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
        SDL_RenderDrawRect(r, &dest);
    }
}

template <typename T, typename V>
inline void set_field(long ptr, V T::* field, V value) {
    T* obj = cast_from_java_handle<T>(ptr);
    if (!obj) return;
    obj->*field = value;
}

template <typename T, typename V>
inline V get_field(long ptr, V T::* field, V default_value) {
    T* obj = cast_from_java_handle<T>(ptr);
    if (!obj) return default_value;
    return obj->*field;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    g_jvm = vm;
    return JNI_VERSION_24;
}

extern "C" void VersionCore() {
    std::ostringstream oss;
    oss << "JavaSDL2 Version: "
    << VERSION_MAJOR
    << "."
    << VERSION_MINOR
    << "."
    << VERSION_PATCH
#if __clang__
    << std::endl
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

    std::cout << oss.str();
}

extern "C" void InitCore() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cout << "SDL2 Init Error: " << SDL_GetError() << std::endl;
    }

    g_renderer = SDL_CreateRenderer(nullptr, -1, SDL_RENDERER_ACCELERATED);
}

extern "C" void QuitCore() {
    SDL_Quit();
}

extern "C" int GetTicksCore() {
    return (int)SDL_GetTicks();
}

extern "C" void HelpCore() {
    std::ostringstream oss;
    oss << "JavaSDL2 Help: "
    << std::endl
    << "COMMON MISTAKES: "
    << std::endl
    << "Program Crashes on MacOS: "
    << std::endl
    << "On MacOS, due to Cocoa requirements, any program that renders a window must be run on the main thread. "
    << "In order to fix this, run java with the -XstartOnFirstThread flag to ensure that the program dosen't crash."
    << std::endl;
    std::cout << oss.str();
}

extern "C" void DebugCore() {
    std::ostringstream oss;
    oss << "JavaSDL2 Debug: "
    << std::endl
    << "Java VM Pointer: "
    << (long)g_jvm
    << std::endl
    << "SDL2 Renderer Pointer: "
    << (long)g_renderer
    << std::endl
    << "This statement is on line: "
    << __LINE__
    << std::endl;
    std::cout << oss.str();
}

extern "C" int GetKeycodeFromNameCore(const char* name) {
    return SDL_GetKeyFromName(name);
}

extern "C" void PlaySoundCore(const char* path) {
    FILE *f = fopen(path, "rb");
    if (!f) { std::cout << "Could not open file at " << path << std::endl; return; }
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
    alBufferData(buffer, AL_FORMAT_STEREO16, audioData, audioSize, OpenALSettings::SAMPLE_RATE_hz);
    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, (ALint)buffer);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcef(source, AL_GAIN, 1.0f);
    alSourcePlay(source);
    printf("Playing %s\n", path);
    ALint state;
    timespec ts {};
    ts.tv_nsec = OpenALSettings::LOOP_DELAY_ns;
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
}

extern "C" long InitWindowCore(const char* title, int width, int height) {
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) { std::cout << "SDL2 Window Init Error: " << SDL_GetError() << std::endl; return -1; }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) { std::cout << "SDL2 Renderer Init Error: " << SDL_GetError() << std::endl; return -1; }
    SDL_RaiseWindow(window);
    SDL_ShowWindow(window);
    SDL_PumpEvents();
    // ReSharper disable CppDFAMemoryLeak
    auto* w = (Window*)malloc(sizeof(Window));
    // ReSharper restore CppDFAMemoryLeak
    w->window = window;
    w->renderer = renderer;
    // ReSharper disable CppDFAMemoryLeak
    return (long)w; // Memory leaked here: Call Window().destroy in java to free
    // ReSharper restore CppDFAMemoryLeak
}

extern "C" void DestroyWindowCore(long ptr) {
    auto* w = (Window*)ptr;
    if (!w) return;
    if (w->window) SDL_DestroyWindow(w->window);
    if (w->renderer) SDL_DestroyRenderer(w->renderer);
    free(w);
}

extern "C" bool WindowPollEventCore(long ptr) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            std::cout << "Closing Window" << std::endl;
            return false;
        }

        if (event.type == SDL_KEYDOWN) {
            int key = event.key.keysym.sym;
            JNIEnv* env;
            g_jvm->AttachCurrentThread((void**)&env, nullptr);
            if (!env) { std::cout << "Could not attach to JVM" << std::endl; return false; }
            jclass c_window = env->FindClass("dev/linkcoder100788/java_sdl2/JavaSDL2$Window");
            jmethodID m_dispatchKey = env->GetStaticMethodID(c_window, "dispatchKey", "(I)V");
            env->CallStaticVoidMethod(c_window, m_dispatchKey, key);
        }
    }
    return true;
}

extern "C" void WindowClearCore(long ptr, int r, int g, int b, int a) {
    auto* w = (Window*)ptr;
    if (!w) return;
    if (!w->renderer) return;
    SDL_SetRenderDrawColor(w->renderer, r, g, b, a);
    SDL_RenderClear(w->renderer);
}

extern "C" void WindowPresentCore(long ptr) {
    auto* w = (Window*)ptr;
    if (!w) return;
    if (!w->renderer) return;
    SDL_RenderPresent(w->renderer);
}

extern "C" void WindowDrawSpriteCore(long ptr, long spritePtr) {
    auto* w = (Window*)ptr;
    if (!w) return;
    if (!w->renderer) return;
    auto* s = (Sprite*)spritePtr;
    if (!s) return;
    if (!s->sprite.surface) return;
    SDL_Rect dest = { s->x, s->y, s->sprite.width, s->sprite.height };
    SDL_Texture* texture = SDL_CreateTextureFromSurface(w->renderer, s->sprite.surface);
    SDL_RenderCopy(w->renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
}

extern "C" void WindowDrawSpriteScaledCore(long ptr, long spritePtr, int scale) {
    auto* w = (Window*)ptr;
    if (!w) return;
    if (!w->renderer) return;
    auto* s = (Sprite*)spritePtr;
    if (!s) return;
    if (!s->sprite.surface) return;
    SDL_Rect dest = { s->x, s->y, s->sprite.width * scale, s->sprite.height * scale };
    SDL_Texture* texture = SDL_CreateTextureFromSurface(w->renderer, s->sprite.surface);
    SDL_RenderCopy(w->renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
}

extern "C" void WindowDrawSpriteBoundedCore(long ptr, long spritePtr, bool bounding) {
    auto* w = (Window*)ptr;
    if (!w) return;
    if (!w->renderer) return;
    auto* s = (Sprite*)spritePtr;
    if (!s) return;
    if (!s->sprite.surface) return;
    SDL_Rect dest = { s->x, s->y, s->sprite.width, s->sprite.height };
    SDL_Texture* texture = SDL_CreateTextureFromSurface(w->renderer, s->sprite.surface);
    SDL_RenderCopy(w->renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    if (bounding) {
        SDL_SetRenderDrawColor(w->renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(w->renderer, &dest);
    }
}

extern "C" void WindowDrawSpriteScaledBoundedCore(long ptr, long spritePtr, int scale, bool bounding) {
    auto* w = (Window*)ptr;
    if (!w) return;
    if (!w->renderer) return;
    auto* s = (Sprite*)spritePtr;
    if (!s) return;
    if (!s->sprite.surface) return;
    SDL_Rect dest = { s->x, s->y, s->sprite.width * scale, s->sprite.height * scale };
    SDL_Texture* texture = SDL_CreateTextureFromSurface(w->renderer, s->sprite.surface);
    SDL_RenderCopy(w->renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    if (bounding) {
        SDL_SetRenderDrawColor(w->renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(w->renderer, &dest);
    }
}

extern "C" long InitSpriteCore(const char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface) { std::cout << "SDL2 Surface Init Error: " << SDL_GetError() << std::endl; return -1; }
    // ReSharper disable CppDFAMemoryLeak
    auto* s = (Sprite*)malloc(sizeof(Sprite));
    // ReSharper restore CppDFAMemoryLeak
    s->sprite.surface = surface;
    s->sprite.width = surface->w;
    s->sprite.height = surface->h;
    s->x = 0;
    s->y = 0;
    s->path = strdup(path);
    // ReSharper disable CppDFAMemoryLeak
    return (long)s; // Memory leaked here: Call Sprite().destroy() to free
    // ReSharper restore CppDFAMemoryLeak
}

extern "C" long InitSpritePosCore(const char* path, int x, int y) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface) { std::cout << "SDL2 Surface Init Error: " << SDL_GetError() << std::endl; return -1; }
    // ReSharper disable CppDFAMemoryLeak
    auto* s = (Sprite*)malloc(sizeof(Sprite));
    // ReSharper restore CppDFAMemoryLeak
    s->sprite.surface = surface;
    s->sprite.width = surface->w;
    s->sprite.height = surface->h;
    s->x = x;
    s->y = y;
    s->path = strdup(path);
    // ReSharper disable CppDFAMemoryLeak
    return (long)s; // Memory leaked here: Call Sprite().destroy() to free
    // ReSharper restore CppDFAMemoryLeak
}

extern "C" void DestroySpriteCore(long ptr) {
    auto* s = (Sprite*)ptr;
    if (!s) return;
    if (s->sprite.surface) SDL_FreeSurface(s->sprite.surface);
    if (s->path) free((void*)s->path);
    free(s);
}

extern "C" void SpriteSetXCore(long ptr, int x) {
    auto* s = (Sprite*)ptr;
    if (!s) return;
    s->x = x;
}

extern "C" void SpriteSetYCore(long ptr, int y) {
    auto* s = (Sprite*)ptr;
    if (!s) return;
    s->y = y;
}

extern "C" long InitOpenALPlayerCore(const char* path) {
    // ReSharper disable CppDFAMemoryLeak
    auto* s = (OpenALPlayer*)malloc(sizeof(OpenALPlayer));
    // ReSharper restore CppDFAMemoryLeak
    if (!s) { std::cout << "Could not allocate OpenALPlayer" << std::endl; return -1; }
    s->path = path;
    // ReSharper disable CppDFAMemoryLeak
    return (long)s; // Memory leaked here: Call OpenALPlayer().destroy() to free
    // ReSharper restore CppDFAMemoryLeak
}

extern "C" void DestroyOpenALPlayerCore(long ptr) {
    auto* s = (OpenALPlayer*)ptr;
    if (!s) return;
    if (!s->path) return;
    free((void*)s->path);
    free(s);
}

extern "C" void OpenALPlayerSetPathCore(long ptr, const char* path) {
    auto* s = (OpenALPlayer*)ptr;
    if (!s) return;
    s->path = path;
}

extern "C" void OpenALPlayerPlayCore(long ptr) {
    auto* s = (OpenALPlayer*)ptr;
    if (!s) return;
    FILE *f = fopen(s->path, "rb");
    if (!f) { std::cout << "Could not open file at " << s->path << std::endl; return; }
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
    alBufferData(buffer, AL_FORMAT_STEREO16, audioData, audioSize, OpenALSettings::SAMPLE_RATE_hz);
    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, (ALint)buffer);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcef(source, AL_GAIN, 1.0f);
    alSourcePlay(source);
    printf("Playing %s\n", s->path);
    ALint state;
    timespec ts {};
    ts.tv_nsec = OpenALSettings::LOOP_DELAY_ns;
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
}

extern "C" long InitScreenCoordinateCore(int x, int y) {
    // ReSharper disable CppDFAMemoryLeak
    auto* s = (ScreenCoordinate*)malloc(sizeof(ScreenCoordinate));
    // ReSharper restore CppDFAMemoryLeak
    s->x = x;
    s->y = y;
    return cast_to_java_handle(s);
}

extern "C" void DestroyScreenCoordinateCore(long ptr) {
    auto* s = cast_from_java_handle<ScreenCoordinate>(ptr);
    if (!s) return;
    free(s);
}

extern "C" void ScreenCoordinateSetXCore(long ptr, int x) {
    set_field<ScreenCoordinate>(ptr, &ScreenCoordinate::x, x);
}

extern "C" void ScreenCoordinateSetYCore(long ptr, int y) {
    set_field<ScreenCoordinate>(ptr, &ScreenCoordinate::y, y);
}

extern "C" int ScreenCoordinateGetXCore(long ptr) {
    return get_field<ScreenCoordinate>(ptr, &ScreenCoordinate::x, -1);
}

extern "C" int ScreenCoordinateGetYCore(long ptr) {
    return get_field<ScreenCoordinate>(ptr, &ScreenCoordinate::y, -1);
}
