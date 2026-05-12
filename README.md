# python_sdl2

> Native SDL2 + OpenAL bindings for **Python**, **Ruby**, and **Java**, powered by a shared C/C++ core.

`python_sdl2` is a small cross-language multimedia binding project that exposes SDL2 windowing/rendering and OpenAL audio playback to multiple runtimes from one native implementation.

The project currently builds:

- A **Python** extension module: `python_sdl2.so`
- A **Ruby** native extension: `ruby_sdl2.bundle`
- A **Java** JNI library and JAR: `java_sdl2.dylib` / `JavaSDL2.jar`

It is useful for experimenting with native language bindings, SDL2 rendering, keyboard input callbacks, simple sprite drawing, and WAV playback through OpenAL.

---

## Table of Contents

- [Features](#features)
- [Supported Languages](#supported-languages)
- [Project Layout](#project-layout)
- [Requirements](#requirements)
- [Installing Dependencies](#installing-dependencies)
  - [macOS](#macos)
  - [Linux](#linux)
  - [Windows](#windows)
- [Building from Source](#building-from-source)
- [Build Outputs](#build-outputs)
- [Running the Examples](#running-the-examples)
  - [Python](#python)
  - [Ruby](#ruby)
  - [Java](#java)
- [API Overview](#api-overview)
  - [Common Concepts](#common-concepts)
  - [Python API](#python-api)
  - [Ruby API](#ruby-api)
  - [Java API](#java-api)
- [Example Usage](#example-usage)
  - [Python Example](#python-example)
  - [Ruby Example](#ruby-example)
  - [Java Example](#java-example)
- [Development Notes](#development-notes)
- [Troubleshooting](#troubleshooting)
- [License](#license)

---

## Features

- Create native SDL2 windows.
- Poll SDL2 events.
- Register keyboard callbacks.
- Clear windows with RGBA colors.
- Draw BMP sprites.
- Draw sprites with scaling and optional bounds visualization.
- Load and play WAV files through OpenAL.
- Query SDL ticks.
- Resolve SDL keycodes from human-readable key names.
- Use a shared C/C++ native core from Python, Ruby, and Java.
- Generate Python reflection glue during the CMake build.
- Build Java JNI bindings and package Java classes into a JAR.

---

## Supported Languages

| Language | Binding Type | Main Import / Entry Point |
|---|---:|---|
| Python | CPython native extension | `import python_sdl2` |
| Ruby | Native extension bundle | `require 'ruby_sdl2'` |
| Java | JNI shared library + JAR | `dev.linkcoder100788.java_sdl2.JavaSDL2` |

---

## Project Layout
```
text
.
├── CMakeLists.txt
├── README.md
├── lib/
│   └── ruby_sdl2.rb
├── scripts/
│   ├── dylibChecker.sh
│   ├── leakCheck_testJava.sh
│   ├── leakCheck_testPython.sh
│   ├── leakCheck_testRuby.sh
│   ├── testJava.sh
│   ├── testPython.sh
│   └── testRuby.sh
├── src/
│   ├── bindings/
│   │   ├── java/
│   │   ├── python/
│   │   └── ruby/
│   └── core/
│       ├── PyCore.cpp
│       ├── PyCore.h
│       ├── RbCore.cpp
│       └── RbCore.h
├── test/
│   ├── assets/
│   ├── Test.java
│   ├── test.py
│   ├── test.rb
│   ├── verbose.py
│   └── verbose.rb
└── tools/
└── reflector.cpp
```
Important directories:

- `src/core/` — shared native implementation used by bindings.
- `src/bindings/python/` — Python extension entry point.
- `src/bindings/ruby/` — Ruby extension entry point.
- `src/bindings/java/` — Java classes and JNI native code.
- `tools/reflector.cpp` — reflection/generation tool used by the Python binding build.
- `test/` — example programs and test assets.
- `scripts/` — convenience scripts for running examples and leak checks.

---

## Requirements

You need:

- **CMake 4.3+**
- A **C++14-compatible compiler**
- **SDL2**
- **OpenAL**
- **Python 3** with development headers
- **Ruby** with development headers
- **Java JDK** with JNI headers
- **LLVM / Clang** for the reflection tool used during the Python build

The project is currently most directly configured for macOS-style development, especially for Java JNI include paths and Homebrew LLVM discovery.

---

## Recommended Installation Method

It is recommended to install the project using homebrew.

Step 1: Tap the repository of your choice
```bash
brew tap link-coder100788/python-sdl2
brew tap link-coder100788/ruby-sdl2
brew tap link-coder100788/java-sdl2
```

Step 2: Install the project
```bash
brew install python-sdl2
brew install ruby-sdl2
brew install java-sdl2
```

The homebrew repositories can be found at:
- [python-sdl2](https://github.com/link-coder100788/homebrew-python-sdl2)
- [ruby-sdl2](https://github.com/link-coder100788/homebrew-ruby-sdl2)
- [java-sdl2](https://github.com/link-coder100788/homebrew-java-sdl2)

## Installing Dependencies & Building From Source

### macOS

Using Homebrew:
```
bash
brew install cmake sdl2 openal-soft llvm python ruby
```
You also need a JDK installed. For example:
```
bash
brew install openjdk
```
If CMake cannot find LLVM automatically, pass the LLVM CMake directory explicitly:
```
bash
cmake -S . -B build -DLLVM_DIR="$(brew --prefix llvm)/lib/cmake/llvm"
```
If OpenAL headers are installed with the OpenAL-soft layout, enable:
```
bash
-DUSE_HOMEBREW_OPENAL=ON
```
Example:
```
bash
cmake -S . -B build \
-DLLVM_DIR="$(brew --prefix llvm)/lib/cmake/llvm" \
-DUSE_HOMEBREW_OPENAL=ON
```
---

### Linux

On Debian/Ubuntu-like systems:
```
bash
sudo apt update
sudo apt install \
build-essential \
cmake \
python3-dev \
ruby-dev \
default-jdk \
libsdl2-dev \
libopenal-dev \
llvm-dev \
clang
```
Depending on your distribution, package names may differ.

---

### Windows

Windows support depends on your local toolchain setup.

Recommended options:

- Install dependencies through **vcpkg**, **MSYS2**, or manually.
- Make sure CMake can find:
  - Python development files
  - Ruby development files
  - SDL2
  - OpenAL
  - LLVM / Clang
  - JDK / JNI headers

You may need to adjust platform-specific JNI include paths in `CMakeLists.txt`.

---

## Building from Source

From the project root:
```
bash
cmake -S . -B build
cmake --build build
```
For macOS with Homebrew LLVM and OpenAL-soft:
```
bash
cmake -S . -B build \
-DLLVM_DIR="$(brew --prefix llvm)/lib/cmake/llvm" \
-DUSE_HOMEBREW_OPENAL=ON

cmake --build build
```
The project also supports the following CMake options:

| Option | Default | Description |
|---|---:|---|
| `BUILD_PYTHON_MODULE` | `ON` | Build the Python module. |
| `USE_HOMEBREW_OPENAL` | `OFF` | Use OpenAL-soft/Homebrew-style header layout. |

---

## Build Outputs

After a successful build, outputs are created in the build directory.

Common outputs include:
```
text
build/
├── JavaSDL2.jar
├── java_classes/
├── libjava_sdl2.dylib
├── python_sdl2.<python-extension-suffix>
├── reflect_tool
└── ruby_sdl2.bundle
```
On macOS, the Python extension may look similar to:
```
text
python_sdl2.cpython-314-darwin.so
```
The exact Python suffix depends on your Python version and platform.

---

## Running the Examples

The repository includes test programs in `test/`.

The examples expect to be run from the `test/` directory or with paths adjusted so assets can be found under `test/assets`.

---

### Python

From the project root, after building:
```
bash
cd test
PYTHONPATH=../build python3 test.py
```
If you are using the default CLion/CMake debug directory from this repository:
```
bash
cd test
PYTHONPATH=../cmake-build-debug python3 test.py
```
---

### Ruby

From the project root:
```
bash
cd test
ruby test.rb
```
The included Ruby test currently loads the extension from the debug build directory using `require_relative`.

If you build into `build/`, update the require path or run with an appropriate load path.

---

### Java

Compile the Java test with the generated JAR:
```
bash
cd test
javac -cp ../build/JavaSDL2.jar Test.java
```
Run it with the native library path set to the build directory:
```
bash
java \
--enable-native-access=ALL-UNNAMED \
-Djava.library.path=../build \
-cp ../build/JavaSDL2.jar:. \
Test
```
On macOS, SDL/Cocoa applications often need to run on the first thread:
```
bash
java \
-XstartOnFirstThread \
--enable-native-access=ALL-UNNAMED \
-Djava.library.path=../build \
-cp ../build/JavaSDL2.jar:. \
Test
```
If you are using the default debug build directory:
```
bash
java \
-XstartOnFirstThread \
--enable-native-access=ALL-UNNAMED \
-Djava.library.path=../cmake-build-debug \
-cp ../cmake-build-debug/JavaSDL2.jar:. \
Test
```
---

## API Overview

### Common Concepts

The bindings expose a similar set of concepts across languages.

| Concept | Description |
|---|---|
| `Window` | SDL2 window wrapper for rendering and event polling. |
| `Sprite` | BMP sprite/texture object that can be drawn in a window. |
| `OpenALPlayer` | Audio player for WAV playback through OpenAL. |
| `ScreenCoordinate` | Simple 2D coordinate object. |
| `init` | Initialize native SDL/OpenAL systems. |
| `quit` | Shut down native systems. |
| `get_ticks` / `getTicks` | Get SDL tick count. |
| `get_keycode_from_name` / `getKeycodeFromName` | Convert key names like `"a"` or `"space"` into SDL keycodes. |
| `play_sound` / `playSound` | Play a sound file directly. |

---

### Python API

Module:
```
python
import python_sdl2
```
Common functions:

- `python_sdl2.version()`
- `python_sdl2.help()`
- `python_sdl2.init()`
- `python_sdl2.quit()`
- `python_sdl2.get_ticks()`
- `python_sdl2.get_keycode_from_name(name)`
- `python_sdl2.play_sound(path)`
- `python_sdl2._debug()`

Common classes:

- `python_sdl2.Window`
- `python_sdl2.Sprite`
- `python_sdl2.OpenALPlayer`
- `python_sdl2.ScreenCoordinate`

Typical methods include:

- `Window.poll_events()`
- `Window.clear(r, g, b, a)`
- `Window.present()`
- `Window.draw_sprite(sprite)`
- `Window.draw_sprite(sprite, scale, show_bounds)`
- `Window.set_key_callback(keycode, callback)`
- `Sprite.set_location(x, y)`
- `Sprite.set_x(x)`
- `Sprite.set_y(y)`
- `OpenALPlayer.play_sound()`
- `OpenALPlayer.set_path(path)`

---

### Ruby API

Module:
```
ruby
require 'ruby_sdl2'
```
Common module methods:

- `RubySDL2.version`
- `RubySDL2.help`
- `RubySDL2.init`
- `RubySDL2.quit`
- `RubySDL2.get_ticks`
- `RubySDL2.get_keycode_from_name(name)`
- `RubySDL2.play_sound(path)`
- `RubySDL2._debug`

Common classes:

- `RubySDL2::Window`
- `RubySDL2::Sprite`
- `RubySDL2::OpenALPlayer`
- `RubySDL2::ScreenCoordinate`

Typical methods include:

- `Window#poll_events`
- `Window#clear(r, g, b, a)`
- `Window#present`
- `Window#draw_sprite(sprite)`
- `Window#draw_sprite(sprite, scale, show_bounds)`
- `Window#set_key_callback(keycode, proc)`
- `Sprite#set_location(x, y)`
- `Sprite#set_x(x)`
- `Sprite#set_y(y)`
- `OpenALPlayer#play_sound`
- `OpenALPlayer#set_path(path)`

---

### Java API

Main class:
```
java
import dev.linkcoder100788.java_sdl2.JavaSDL2;
```
Nested classes:

- `JavaSDL2.Window`
- `JavaSDL2.Sprite`
- `JavaSDL2.OpenALPlayer`
- `JavaSDL2.ScreenCoordinate`

Common static methods:

- `JavaSDL2.version()`
- `JavaSDL2.help()`
- `JavaSDL2.init()`
- `JavaSDL2.quit()`
- `JavaSDL2.getTicks()`
- `JavaSDL2.getKeycodeFromName(String name)`
- `JavaSDL2.playSound(String path)`
- `JavaSDL2._debug()`

Typical instance methods include:

- `Window.pollEvent()`
- `Window.clear(int r, int g, int b, int a)`
- `Window.present()`
- `Window.drawSpriteScaledBounded(Sprite sprite, int scale, boolean showBounds)`
- `Window.onKey(int keycode, callback)`
- `Window.destroy()`
- `Sprite.setX(int x)`
- `Sprite.setY(int y)`
- `Sprite.destroy()`
- `OpenALPlayer.playSound()`
- `OpenALPlayer.setPath(String path)`
- `OpenALPlayer.destroy()`

---

## Example Usage

### Python Example
```
python
import python_sdl2
import math
import time

python_sdl2.init()

window = python_sdl2.Window("Python SDL2 Example", 800, 600)
sprite = python_sdl2.Sprite("assets/aa.bmp")

x = 400
y = 300

def move_left(key):
global x
x -= 5

def move_right(key):
global x
x += 5

def move_up(key):
global y
y -= 5

def move_down(key):
global y
y += 5

window.set_key_callback(python_sdl2.get_keycode_from_name("a"), move_left)
window.set_key_callback(python_sdl2.get_keycode_from_name("d"), move_right)
window.set_key_callback(python_sdl2.get_keycode_from_name("w"), move_up)
window.set_key_callback(python_sdl2.get_keycode_from_name("s"), move_down)

running = True
t = 0.0

while running:
running = window.poll_events()

    r = int(abs(math.sin(t)) * 255)
    g = int(abs(math.cos(t)) * 255)

    window.clear(r, g, 150, 255)
    sprite.set_location(x, y)
    window.draw_sprite(sprite, 5, True)
    window.present()

    t += (math.pi / 4.0) / 60.0
    time.sleep(0.016)

python_sdl2.quit()
```
---

### Ruby Example
```
ruby
require 'ruby_sdl2'

RubySDL2.init

window = RubySDL2::Window.new("Ruby SDL2 Example", 800, 600)
sprite = RubySDL2::Sprite.new("assets/aa.bmp")

state = {
x: 400,
y: 300,
running: true,
t: 0.0
}

window.set_key_callback(RubySDL2.get_keycode_from_name("a"), proc { state[:x] -= 5 })
window.set_key_callback(RubySDL2.get_keycode_from_name("d"), proc { state[:x] += 5 })
window.set_key_callback(RubySDL2.get_keycode_from_name("w"), proc { state[:y] -= 5 })
window.set_key_callback(RubySDL2.get_keycode_from_name("s"), proc { state[:y] += 5 })

while state[:running]
state[:running] = false unless window.poll_events

r = (Math.sin(state[:t]).abs * 255).to_i
g = (Math.cos(state[:t]).abs * 255).to_i

window.clear(r, g, 150, 255)
sprite.set_location(state[:x], state[:y])
window.draw_sprite(sprite, 5, true)
window.present

state[:t] += (Math::PI / 4.0) / 60.0
sleep 0.016
end

RubySDL2.quit
```
---

### Java Example
```
java
import dev.linkcoder100788.java_sdl2.JavaSDL2;
import dev.linkcoder100788.java_sdl2.JavaSDL2.Window;
import dev.linkcoder100788.java_sdl2.JavaSDL2.Sprite;

public class Test {
public static void main(String[] args) {
JavaSDL2.init();

        Window window = new Window("Java SDL2 Example", 800, 600);
        Sprite sprite = new Sprite("assets/aa.bmp");

        final int[] x = {400};
        final int[] y = {300};

        window.onKey(JavaSDL2.getKeycodeFromName("a"), key -> x[0] -= 5);
        window.onKey(JavaSDL2.getKeycodeFromName("d"), key -> x[0] += 5);
        window.onKey(JavaSDL2.getKeycodeFromName("w"), key -> y[0] -= 5);
        window.onKey(JavaSDL2.getKeycodeFromName("s"), key -> y[0] += 5);

        boolean running = true;
        double t = 0.0;

        while (running) {
            running = window.pollEvent();

            int r = (int)(Math.abs(Math.sin(t)) * 255);
            int g = (int)(Math.abs(Math.cos(t)) * 255);

            window.clear(r, g, 150, 255);
            sprite.setX(x[0]);
            sprite.setY(y[0]);
            window.drawSpriteScaledBounded(sprite, 5, true);
            window.present();

            t += (Math.PI / 4.0) / 60.0;

            try {
                Thread.sleep(16);
            } catch (InterruptedException error) {
                Thread.currentThread().interrupt();
                break;
            }
        }

        sprite.destroy();
        window.destroy();
        JavaSDL2.quit();
    }
}
```
---

## Development Notes

### Python Reflection Generation

The Python binding uses a native reflection helper:
```
text
reflect_tool
```
During the CMake build, this tool reads the core Python-facing header and generates binding glue files into the build directory:
```
text
build/generated/
├── generated_reflect.c
└── generated_reflect.h
```
These generated files are then compiled into the Python extension module.

---

### Java JNI

The Java binding consists of:

- Java source under `src/bindings/java/`
- Native JNI implementation under `src/bindings/java/native/`
- Generated JNI headers
- A shared native library
- A generated `JavaSDL2.jar`

The build produces:
```
text
JavaSDL2.jar
libjava_sdl2.dylib
```
On Linux, the shared library name will usually differ, such as:
```
text
libjava_sdl2.so
```
On Windows, it may be:
```
text
java_sdl2.dll
```
---

### Warnings as Errors

The native targets are compiled with warnings treated as errors:
```
text
-Werror
```
This helps keep the C/C++ code clean, but it may expose compiler- or platform-specific warnings when building on a new system.

---

## Troubleshooting

### CMake cannot find LLVM

Pass `LLVM_DIR` manually:
```
bash
cmake -S . -B build -DLLVM_DIR="$(brew --prefix llvm)/lib/cmake/llvm"
```
On Linux, locate your LLVM CMake package directory and pass that path instead.

---

### OpenAL headers are not found on macOS

Try enabling the Homebrew/OpenAL-soft layout option:
```
bash
cmake -S . -B build -DUSE_HOMEBREW_OPENAL=ON
```
---

### Python cannot import `python_sdl2`

Make sure the build directory is on `PYTHONPATH`:
```
bash
PYTHONPATH=../build python3 test.py
```
Or, for the default debug build directory:
```
bash
PYTHONPATH=../cmake-build-debug python3 test.py
```
Also confirm that the built extension exists:
```
bash
ls build/python_sdl2*
```
---

### Ruby cannot load `ruby_sdl2`

Make sure Ruby can find the native extension bundle.

For example:
```
ruby
require_relative '../build/ruby_sdl2'
```
or run Ruby with an adjusted load path:
```
bash
ruby -I../build test.rb
```
---

### Java cannot find the native library

Make sure `java.library.path` points to the directory containing the native library:
```
bash
-Djava.library.path=../build
```
Also make sure the JAR is on the classpath:
```
bash
-cp ../build/JavaSDL2.jar:.
```
On Windows, use `;` instead of `:` as the classpath separator.

---

### Java window does not open correctly on macOS

Run Java with:
```
bash
-XstartOnFirstThread
```
Example:
```
bash
java \
-XstartOnFirstThread \
--enable-native-access=ALL-UNNAMED \
-Djava.library.path=../build \
-cp ../build/JavaSDL2.jar:. \
Test
```
---

### Assets cannot be loaded

The examples use relative paths such as:
```
text
assets/aa.bmp
assets/ae.bmp
assets/dingSound.wav
assets/dieSound.wav
```
Run examples from the `test/` directory, or adjust asset paths accordingly.

---

## License

MIT License.
