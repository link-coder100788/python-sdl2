#include <stdbool.h>
#include "dev_linkcoder100788_java_sdl2_JavaSDL2.h"
#include "dev_linkcoder100788_java_sdl2_JavaSDL2_Window.h"
#include "dev_linkcoder100788_java_sdl2_JavaSDL2_Sprite.h"
#include "dev_linkcoder100788_java_sdl2_JavaSDL2_OpenALPlayer.h"
#include "dev_linkcoder100788_java_sdl2_JavaSDL2_ScreenCoordinate.h"
#include "JCore.h"

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_version(JNIEnv* env, jclass jclass) {
    VersionCore();
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_init(JNIEnv* env, jclass jclass) {
    InitCore();
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_quit(JNIEnv* env, jclass jclass) {
    QuitCore();
}

JNIEXPORT jint JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_getTicks(JNIEnv* env, jclass jclass) {
    return GetTicksCore();
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_help(JNIEnv* env, jclass jclass) {
    HelpCore();
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2__1debug(JNIEnv* env, jclass jclass) {
    DebugCore();
}

JNIEXPORT jint JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_getKeycodeFromName(JNIEnv* env, jclass jclass, jstring s1) {
    const char* string = (*env)->GetStringUTFChars(env, s1, NULL);
    if (!string) return -1;
    int kc = GetKeycodeFromNameCore(string);
    (*env)->ReleaseStringUTFChars(env, s1, string);
    return kc;
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_playSound(JNIEnv* env, jclass jclass, jstring s1) {
    const char* string = (*env)->GetStringUTFChars(env, s1, NULL);
    if (!string) return;
    PlaySoundCore(string);
    (*env)->ReleaseStringUTFChars(env, s1, string);
}

JNIEXPORT jlong JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Window_initWindow(JNIEnv* env, jclass jclass, jstring s1, jint i1, jint i2) {
    const char* string = (*env)->GetStringUTFChars(env, s1, NULL);
    if (!string) return -1;
    long l = InitWindowCore(string, i1, i2);
    (*env)->ReleaseStringUTFChars(env, s1, string);
    return l;
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Window_destroyWindow(JNIEnv* env, jclass jclass, jlong l1) {
    DestroyWindowCore(l1);
}

JNIEXPORT jboolean JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Window_pollEvent(JNIEnv* env, jclass jclass, jlong l1) {
    return WindowPollEventCore(l1);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Window_clear(JNIEnv* env, jclass jclass, jlong l1, jint i1, jint i2, jint i3, jint i4) {
    WindowClearCore(l1, i1, i2, i3, i4);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Window_present(JNIEnv* env, jclass jclass, jlong l1) {
    WindowPresentCore(l1);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Window_drawSprite(JNIEnv* env, jclass jclass, jlong l1, jlong l2) {
    WindowDrawSpriteCore(l1, l2);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Window_drawSpriteScaled(JNIEnv* env, jclass jclass, jlong l1, jlong l2, jint i1) {
    WindowDrawSpriteScaledCore(l1, l2, i1);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Window_drawSpriteBounded(JNIEnv* env, jclass jclass, jlong l1, jlong l2, jboolean b1) {
    WindowDrawSpriteBoundedCore(l1, l2, b1);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Window_drawSpriteScaledBounded(JNIEnv* env, jclass jclass, jlong l1, jlong l2, jint i1, jboolean b1) {
    WindowDrawSpriteScaledBoundedCore(l1, l2, i1, b1);
}

JNIEXPORT jlong JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Sprite_initSprite(JNIEnv* env, jclass jclass, jstring s1) {
    const char* string = (*env)->GetStringUTFChars(env, s1, NULL);
    if (!string) return -1;
    long l = InitSpriteCore(string);
    (*env)->ReleaseStringUTFChars(env, s1, string);
    return l;
}

JNIEXPORT jlong JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Sprite_initSpritePos(JNIEnv* env, jclass jclass, jstring s1, jint i1, jint i2) {
    const char* string = (*env)->GetStringUTFChars(env, s1, NULL);
    if (!string) return -1;
    long l = InitSpritePosCore(string, i1, i2);
    (*env)->ReleaseStringUTFChars(env, s1, string);
    return l;
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Sprite_destroySprite(JNIEnv* env, jclass jclass, jlong l1) {
    DestroySpriteCore(l1);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Sprite_setX(JNIEnv* env, jclass jclass, jlong l1, jint i1) {
    SpriteSetXCore(l1, i1);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024Sprite_setY(JNIEnv* env, jclass jclass, jlong l1, jint i1) {
    SpriteSetYCore(l1, i1);
}

JNIEXPORT jlong JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024OpenALPlayer_initOpenALPlayer(JNIEnv* env, jclass jclass, jstring s1) {
    const char* string = (*env)->GetStringUTFChars(env, s1, NULL);
    if (!string) return -1;
    char* string2 = malloc(strlen(string) + 1);
    strcpy(string2, string);
    long l = InitOpenALPlayerCore(string2);
    (*env)->ReleaseStringUTFChars(env, s1, string);
    return l;
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024OpenALPlayer_destroyOpenALPlayer(JNIEnv* env, jclass jclass, jlong l1) {
    DestroyOpenALPlayerCore(l1);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024OpenALPlayer_setPath(JNIEnv* env, jclass jclass, jlong l1, jstring s1) {
    const char* string = (*env)->GetStringUTFChars(env, s1, NULL);
    if (!string) return;
    char* string2 = malloc(strlen(string) + 1);
    strcpy(string2, string);
    OpenALPlayerSetPathCore(l1, string2);
    (*env)->ReleaseStringUTFChars(env, s1, string);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024OpenALPlayer_playSound(JNIEnv* env, jclass jclass, jlong l1) {
    OpenALPlayerPlayCore(l1);
}

JNIEXPORT jlong JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024ScreenCoordinate_initScreenCoordinate(JNIEnv* env, jclass jclass, jint i1, jint i2) {
    return InitScreenCoordinateCore(i1, i2);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024ScreenCoordinate_destroyScreenCoordinate(JNIEnv* env, jclass jclass, jlong l1) {
    DestroyScreenCoordinateCore(l1);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024ScreenCoordinate_setX(JNIEnv* env, jclass jclass, jlong l1, jint i1) {
    ScreenCoordinateSetXCore(l1, i1);
}

JNIEXPORT void JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024ScreenCoordinate_setY(JNIEnv* env, jclass jclass, jlong l1, jint i1) {
    ScreenCoordinateSetYCore(l1, i1);
}

JNIEXPORT jint JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024ScreenCoordinate_getX(JNIEnv* env, jclass jclass, jlong l1) {
    return ScreenCoordinateGetXCore(l1);
}

JNIEXPORT jint JNICALL Java_dev_linkcoder100788_java_1sdl2_JavaSDL2_00024ScreenCoordinate_getY(JNIEnv* env, jclass jclass, jlong l1) {
    return ScreenCoordinateGetYCore(l1);
}