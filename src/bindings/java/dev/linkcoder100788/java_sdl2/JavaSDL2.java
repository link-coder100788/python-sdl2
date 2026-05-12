package dev.linkcoder100788.java_sdl2;

import java.util.HashMap;
import java.util.function.Consumer;

public class JavaSDL2 {
    static {
        System.loadLibrary("java_sdl2");
    }

    public static native void version();
    public static native void init();
    public static native void quit();
    public static native int getTicks();
    public static native void help();
    public static native void _debug();
    public static native int getKeycodeFromName(String name);
    public static native void playSound(String path);

    public static class Window {
        private static void _$dGenericInfo() {
            System.out.println("Window");
        }

        @java.lang.Override
        public java.lang.String toString() {
            return "Window{" +
                    "_handle=" + _handle +
                    '}';
        }

        private long _handle;

        private final HashMap<Integer, Consumer<Integer>> keyCallbacks = new HashMap<>();

        private static Window _INSTANCE;

        private static native long initWindow(String title, int x, int y);
        private static native void destroyWindow(long handle);

        public Window(String title, int x, int y) {
            this._handle = initWindow(title, x, y);
            _INSTANCE = this;
        }

        public void destroy() {
            _INSTANCE = null;
            destroyWindow(_handle);
        }

        public long _getHandle() {
            return _handle;
        }

        public void onKey(int keyCode, Consumer<Integer> callback) {
            keyCallbacks.put(keyCode, callback);
        }

        private void handleKey(int keyCode) {
            Consumer<Integer> cb = keyCallbacks.get(keyCode);
            if (cb != null) cb.accept(keyCode);
        }

        private static void dispatchKey(int keyCode) {
            if (_INSTANCE != null) {
                _INSTANCE.handleKey(keyCode);
            }
        }

        private static native boolean pollEvent(long handle);
        private static native void clear(long handle, int r, int g, int b, int a);
        private static native void present(long handle);
        private static native void drawSprite(long handle, long spriteHandle);
        private static native void drawSpriteScaled(long handle, long spriteHandle, int scale);
        private static native void drawSpriteBounded(long handle, long spriteHandle, boolean bounded);
        private static native void drawSpriteScaledBounded(long handle, long spriteHandle, int scale, boolean bounded);

        public boolean pollEvent() {
            return pollEvent(_handle);
        }

        public void clear(int r, int g, int b, int a) {
            clear(_handle, r, g, b, a);
        }

        public void present() {
            present(_handle);
        }

        public void drawSprite(Sprite sprite) {
            drawSprite(_handle, sprite._getHandle());
        }

        public void drawSpriteScaled(Sprite sprite, int scale) {
            drawSpriteScaled(_handle, sprite._getHandle(), scale);
        }

        public void drawSpriteBounded(Sprite sprite, boolean bounded) {
            drawSpriteBounded(_handle, sprite._getHandle(), bounded);
        }

        public void drawSpriteScaledBounded(Sprite sprite, int scale, boolean bounded) {
            drawSpriteScaledBounded(_handle, sprite._getHandle(), scale, bounded);
        }
    }

    public static class Sprite {
        @java.lang.Override
        public java.lang.String toString() {
            return "Sprite{" +
                    "_handle=" + _handle +
                    '}';
        }

        private long _handle;

        private static native long initSprite(String path);
        private static native long initSpritePos(String path, int x, int y);
        private static native void destroySprite(long handle);

        public Sprite(String path) {
            this._handle = initSprite(path);
        }

        public Sprite(String path, int x, int y) {
            this._handle = initSpritePos(path, x, y);
        }

        public void destroy() {
            destroySprite(_handle);
        }

        public long _getHandle() {
            return _handle;
        }

        private static native void setX(long handle, int x);
        private static native void setY(long handle, int y);

        public void setX(int x) {
            setX(_handle, x);
        }

        public void setY(int y) {
            setY(_handle, y);
        }
    }

    public static class OpenALPlayer {
        @java.lang.Override
        public java.lang.String toString() {
            return "OpenALPlayer{" +
                    "_handle=" + _handle +
                    '}';
        }

        private long _handle;

        private static native long initOpenALPlayer(String path);
        private static native void destroyOpenALPlayer(long handle);

        public OpenALPlayer(String path) {
            _handle = initOpenALPlayer(path);
        }

        public void destroy() {
            destroyOpenALPlayer(_handle);
        }

        private static native void setPath(long handle, String path);
        private static native void playSound(long handle);

        public void setPath(String path) {
            setPath(_handle, path);
        }

        public void playSound() {
            playSound(_handle);
        }
    }

    public static class ScreenCoordinate {
        @java.lang.Override
        public java.lang.String toString() {
            return "ScreenCoordinate{" +
                    "_handle=" + _handle +
                    '}';
        }

        private long _handle;

        private static native long initScreenCoordinate(int x, int y);
        private static native void destroyScreenCoordinate(long handle);

        public ScreenCoordinate(int x, int y) {
            _handle = initScreenCoordinate(x, y);
        }

        public void destroy() {
            destroyScreenCoordinate(_handle);
        }

        private static native void setX(long handle, int x);
        private static native void setY(long handle, int y);
        private static native int getX(long handle);
        private static native int getY(long handle);

        public void setX(int x) {
            setX(_handle, x);
        }

        public void setY(int y) {
            setY(_handle, y);
        }

        public int getX() {
            return getX(_handle);
        }

        public int getY() {
            return getY(_handle);
        }
    }
}