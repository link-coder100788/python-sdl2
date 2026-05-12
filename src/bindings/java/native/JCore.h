#ifndef PYTHON_SDL2_JCORE_H
#define PYTHON_SDL2_JCORE_H
#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
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

#define JAVASDL2_VERSION VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH

void VersionCore();

void InitCore();

void QuitCore();

int GetTicksCore();

void HelpCore();

void DebugCore();

int GetKeycodeFromNameCore(const char* name);

void PlaySoundCore(const char* path);

struct Window {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

/**
 * Initializes an SDL2 window and renderer with the provided parameters.
 * Creates a window and an associated renderer, centers the window on the screen,
 * raises it to the front, and displays it. Returns a pointer to an allocated
 * Window structure, cast to a long, or -1 in case of an error. The caller is
 * responsible for managing and freeing the allocated Window using the appropriate
 * destroy method.
 *
 * @param title The title of the window to be created.
 * @param width The width of the window to be created.
 * @param height The height of the window to be created.
 * @return A long representing a pointer to the allocated Window structure,
 *         or -1 if an error occurred during initialization.
 */
long InitWindowCore(const char* title, int width, int height);

/**
 * Destroys an SDL2 window and renderer associated with the provided pointer
 * and frees the allocated memory for the Window structure.
 * Ensures that all resources such as the window and renderer are properly released
 * to prevent leaks or undefined behavior.
 *
 * @param ptr A long value representing a pointer to a Window structure
 *            that contains an SDL_Window and SDL_Renderer to be destroyed.
 */
void DestroyWindowCore(long ptr);

bool WindowPollEventCore(long ptr);

void WindowClearCore(long ptr, int r, int g, int b, int a);

void WindowPresentCore(long ptr);

void WindowDrawSpriteCore(long ptr, long spritePtr);

void WindowDrawSpriteScaledCore(long ptr, long spritePtr, int scale);

void WindowDrawSpriteBoundedCore(long ptr, long spritePtr, bool bounding);

void WindowDrawSpriteScaledBoundedCore(long ptr, long spritePtr, int scale, bool bounding);

struct Sprite {
    int x, y;
    const char* path;
    struct {
        SDL_Surface* surface;
        int width, height;
    } sprite;
};

/**
 * Initializes a Sprite structure by loading a bitmap from the specified file path.
 * Allocates memory for a Sprite object, loads the bitmap as an SDL_Surface, and sets
 * the properties of the Sprite, including the surface, its dimensions, and the file path.
 * The caller is responsible for managing and freeing the allocated Sprite using the
 * appropriate destroy method to avoid memory leaks.
 *
 * @param path The file path of the bitmap to be loaded.
 * @return A long representing a pointer to the allocated Sprite structure,
 *         or -1 if an error occurred during initialization.
 */
long InitSpriteCore(const char* path);

/**
 * Initializes a Sprite structure with the specified image file and position.
 * Loads an SDL2 surface from the given file path, creates a Sprite structure,
 * and assigns the provided x and y coordinates to the sprite. The file path is
 * stored within the sprite for reference. Returns a pointer to the allocated
 * Sprite structure, cast to a long, or -1 in case of an error. The caller is
 * responsible for freeing the allocated memory using an appropriate destroy method.
 *
 * @param path The file path of the BMP image to load into the sprite.
 * @param x The x-coordinate to initialize the sprite's position.
 * @param y The y-coordinate to initialize the sprite's position.
 * @return A long representing a pointer to the allocated Sprite structure,
 *         or -1 if an error occurred during initialization.
 */
long InitSpritePosCore(const char* path, int x, int y);

/**
 * Deallocates a Sprite structure and its associated resources.
 * Frees the memory allocated for a Sprite object, including its
 * surface and file path, if applicable. This function ensures all
 * allocated resources linked to the Sprite are properly released,
 * preventing memory leaks.
 *
 * @param ptr A long representing the pointer to the Sprite structure
 *            to be destroyed. If the pointer is null, the function
 *            does nothing.
 */
void DestroySpriteCore(long ptr);

void SpriteSetXCore(long ptr, int x);

void SpriteSetYCore(long ptr, int y);

struct OpenALPlayer {
    const char* path;
};

/**
 * Initializes an OpenALPlayer instance with the provided file path.
 * Allocates memory for an OpenALPlayer structure and sets its path attribute
 * to the given file path. Returns a pointer to the allocated OpenALPlayer structure,
 * cast to a long. The caller is responsible for freeing the allocated memory
 * by calling the appropriate destroy method.
 *
 * @param path The file path associated with the OpenALPlayer instance.
 * @return A long representing a pointer to the allocated OpenALPlayer structure,
 *         or -1 if memory allocation failed.
 */
long InitOpenALPlayerCore(const char* path);

/**
 * Destroys an OpenALPlayer instance and releases its allocated resources.
 * Frees the memory associated with the OpenALPlayer structure and its path.
 * The pointer input must be a valid OpenALPlayer instance, otherwise the method
 * will exit without performing any operations.
 *
 * @param ptr A long representing a pointer to the OpenALPlayer structure to be destroyed.
 */
void DestroyOpenALPlayerCore(long ptr);

void OpenALPlayerSetPathCore(long ptr, const char* path);

void OpenALPlayerPlayCore(long ptr);

struct ScreenCoordinate {
    int x, y;
};

/**
 * Allocates and initializes a ScreenCoordinate structure with the specified
 * x and y values. The created structure's memory is dynamically allocated,
 * and the caller is responsible for managing and freeing it appropriately.
 * The function returns a handle (long) that represents a pointer to the
 * allocated ScreenCoordinate structure.
 *
 * @param x The x-coordinate to initialize in the ScreenCoordinate structure.
 * @param y The y-coordinate to initialize in the ScreenCoordinate structure.
 * @return A long representing a pointer to the allocated ScreenCoordinate structure.
 */
long InitScreenCoordinateCore(int x, int y);

/**
 * Destroys a ScreenCoordinate instance referenced by the given pointer.
 * Frees the memory allocated for the ScreenCoordinate object, ensuring
 * proper cleanup. The pointer must refer to a valid ScreenCoordinate instance,
 * or the function will exit without performing any operation.
 *
 * @param ptr A long value representing a pointer to the ScreenCoordinate instance
 *            to be destroyed. If the pointer is invalid, the method will return
 *            without performing any action.
 */
void DestroyScreenCoordinateCore(long ptr);

void ScreenCoordinateSetXCore(long ptr, int x);

void ScreenCoordinateSetYCore(long ptr, int y);

int ScreenCoordinateGetXCore(long ptr);

int ScreenCoordinateGetYCore(long ptr);

#ifdef __cplusplus
}
#endif

#endif //PYTHON_SDL2_JCORE_H
