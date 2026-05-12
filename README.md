# python_sdl2 / RubySDL2

Cross-language SDL2 and OpenAL bindings that expose the same core engine-style API to both Python and Ruby.

## Overview

This project provides native extension modules for:

- **Python** as `python_sdl2`
- **Ruby** as `ruby_sdl2`

It is designed around a small set of shared concepts:

- `Window` for rendering and event handling.
- `Sprite` for drawable objects.
- `ScreenCoordinate` for 2D positions.
- `OpenALPlayer` for audio playback.

The Python binding uses a generated reflection layer, while the Ruby binding is implemented directly against the shared core code.

## Features

- SDL2 window creation and management.
- Event polling and key input helpers.
- Sprite drawing support.
- Screen clearing and buffer presentation.
- OpenAL sound playback.
- Shared SDL2/OpenAL core logic across both language bindings.

## Requirements

- CMake 4.3 or newer.
- A C++14-compatible compiler.
- Python 3 with development headers.
- Ruby with development headers.
- SDL2.
- OpenAL.
- LLVM and Clang for the Python reflection tool.

## Installation

### macOS

Install the ruby or python bindings with Homebrew:

```bash
brew tap link-coder100788/python-sdl2
brew tap link-coder100788/ruby-sdl2

brew install python-sdl2
brew install ruby-sdl2
```

## Build from source

### macOS

Install the dependencies with Homebrew:

```bash
brew install sdl2 openal-soft llvm python ruby cmake
```

If CMake cannot find LLVM, you may need to point it at Homebrew's LLVM installation:

```bash
cmake -S . -B build -DLLVM_DIR="$(brew --prefix llvm)/lib/cmake/llvm"
```

### Linux

On Debian or Ubuntu, install the dependencies with apt:

```bash
sudo apt update
sudo apt install build-essential cmake python3-dev ruby-dev libsdl2-dev libopenal-dev llvm-dev clang
```

If your distribution packages LLVM and Clang separately, install both.

### Windows

Use a package manager such as vcpkg or install the dependencies manually, then configure CMake so it can find Python, Ruby, SDL2, OpenAL, and LLVM.

## Build

```bash
cmake -S . -B build
cmake --build build
```

The project builds both bindings by default. If you want to build only one binding, use the CMake options provided by the project as needed.

## Generated artifacts

The build creates a small reflection generator executable and uses it to produce the Python binding glue code at build time.

Expected outputs include:

- `python_sdl2` Python extension with the correct platform suffix.
- `ruby_sdl2.bundle` Ruby extension.
- `reflect_tool` helper executable.

## Python usage

```python
import python_sdl2

print(python_sdl2.version())
python_sdl2.init()
window = python_sdl2.create_window(...)
```

### Python types

- `python_sdl2.Window`
- `python_sdl2.OpenALPlayer`
- `python_sdl2.Sprite`
- `python_sdl2.ScreenCoordinate`

### Python functions

- `version()`
- `init()`
- `quit()`
- `create_window(...)`
- `pump_window()`
- `get_ticks()`
- `help()`
- `play_sound(...)`
- `_debug()`
- `get_keycode_from_name(...)`

## Ruby usage

```ruby
require 'ruby_sdl2'

puts RubySDL2.version
RubySDL2.init
window = RubySDL2::Window.new(...)
```

### Ruby classes

- `RubySDL2::Window`
- `RubySDL2::OpenALPlayer`
- `RubySDL2::Sprite`
- `RubySDL2::ScreenCoordinate`

### Ruby module functions

- `version`
- `init`
- `quit`
- `get_ticks`
- `help`
- `_debug`
- `get_keycode_from_name(...)`
- `play_sound(...)`

## Error handling

Both bindings expose the same custom errors:

- `SDLError`
- `OpenALError`
- `ArgumentError`

## Project layout

- `src/bindings/` — Python and Ruby binding entry points.
- `src/core/` — shared SDL2/OpenAL core implementation.
- `tools/reflector.cpp` — reflection generator used for Python bindings.
- `generated/` — build-time generated reflection files.

## Examples

#### Python Examples

```python
import python_sdl2
import time

print("Version info:")
print(python_sdl2.version())

python_sdl2.init()

win = python_sdl2.Window("Render Test", 800, 600)

sprite = python_sdl2.Sprite("test/assets/ae.bmp")

def on_a(key):
    print("A pressed!")

a_code = python_sdl2.get_keycode_from_name("a")

win.set_key_callback(a_code, on_a)

running = True

while running:
    running = win.poll_events()
    win.clear(128, 128, 128, 255) # r, g, b, a
    win.draw_sprite(sprite, 5, True) # sprite, scale, bounding box
    win.present()
    time.sleep(0.5)

python_sdl2.quit()
```

To Run:

```bash
PYTHONPATH=path/to/o=python_sdl2 python3 test.py
```

#### Ruby Example

```ruby
require 'ruby_sdl2'

puts "Version info:"
puts RubySDL2.version

RubySDL2.init

win = RubySDL2::Window.new("Render Test", 800, 600)

sprite = RubySDL2::Sprite.new("test/assets/ae.bmp")

win.set_key_callback(RubySDL2.get_keycode_from_name("a"), proc { puts "A pressed!" })

$running = true

while running
  $running = false unless win.poll_events
  win.clear(128, 128, 128, 255) # r, g, b, a
  win.draw_sprite(sprite, 5, true) # sprite, scale, bounding box
  win.present
  sleep 0.5
end

RubySDL2.quit
```

To Run:

```bash
ruby test.rb
```

#### Java Example

```java
import dev.linkcoder100788.java_sdl2.JavaSDL2;
import dev.linkcoder100788.java_sdl2.JavaSDL2.Window;
import dev.linkcoder100788.java_sdl2.JavaSDL2.Sprite;
import java.util.Random;

public class Test {
    public static void main(String[] args) {
        JavaSDL2.version();
        JavaSDL2.help();
        System.out.println("GoodBye!");
        JavaSDL2.init();
        JavaSDL2.playSound("assets/dingSound.wav");
        Window w = new Window("Hello, World!", 600, 600);
        Sprite s = new Sprite("assets/aa.bmp");
        Random r = new Random();
        System.out.println("Window: " + w.toString());
        System.out.println("Sprite: " + s.toString());
        w.onKey(JavaSDL2.getKeycodeFromName("a"), key -> {
            System.out.println("A pressed");
        });
        w.onKey(JavaSDL2.getKeycodeFromName("w"), key -> {
            s.setY(r.nextInt(600));
        });
        w.onKey(JavaSDL2.getKeycodeFromName("s"), key -> {
            s.setY(r.nextInt(600));
        });
        w.onKey(JavaSDL2.getKeycodeFromName("space"), key -> {
            System.out.println("Space pressed");
        });
        s.setY(r.nextInt(600));
        boolean running = true;
        double t = 0;
        while (running) {
            running = w.pollEvent();
            w.clear((int)(Math.abs(Math.sin(t)) * 255), (int)(Math.abs(Math.cos(t)) * 255), 150, 255);
            w.drawSpriteScaledBounded(s, 5, true);
            w.present();
            t += ((Math.PI / 4) / 60);
            try {
                Thread.sleep(16);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        w.destroy();
        s.destroy();
    }
```

To Run:

```bash
javac -cp path/to/JavaSDL2.jar Test.java
# -XstartOnFirstThread is required for macOS due to Cocoa requirements.
java -XstartOnFirstThread \
--enable-native-access=ALL-UNNAMED \
-Djava.library.path=path/to/libjava_sdl2.dylib \
-cp path/to/JavaSDL2.jar Test
```

## Notes

- The Python module name is `python_sdl2.cpython-314-darwin.so`.
- The Ruby extension name is `ruby_sdl2.bundle`.
- The build treats warnings as errors.

## License

MIT Licence