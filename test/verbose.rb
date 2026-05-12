require_relative '../cmake-build-debug/ruby_sdl2' # Adjust path to where your .bundle or .so is located

class Tester
    def initialize

    end

    def test_main
        puts "--- Testing RubySDL2 Module ---"
        puts RubySDL2.help
        puts RubySDL2.version
        puts "Ticks: #{RubySDL2.get_ticks}"

        # 2. Initialization
        begin
          RubySDL2.init
          puts "SDL2 Initialized successfully."
        rescue => e
          puts "Failed to initialize SDL2: #{e.message}"
          exit
        end

        # 3. ScreenCoordinate Test
        puts "\n--- Testing ScreenCoordinate ---"
        coord = RubySDL2::ScreenCoordinate.new(100, 200)
        puts "Coord X: #{coord.x}, Y: #{coord.y}"

        # 4. Sprite Test (Requires a 'test.bmp' in the same folder)
        puts "\n--- Testing Sprite ---"
        begin
          # Creating a sprite at x:50, y:50
          # Note: Ensure you have a small BMP file named 'test.bmp'
          sprite = RubySDL2::Sprite.new("test.bmp", 50, 50)
          puts "Sprite loaded successfully."
        rescue => e
          puts "Sprite Load Error (expected if test.bmp is missing): #{e.message}"
        end
    end



class Tester2
    def run
        require_relative '../cmake-build-debug/ruby_sdl2'

        # =========================================================
        # 1. MODULE INSPECTION
        # =========================================================
        module_info = RubySDL2

        puts "--- RubySDL2 Module Inspection ---"
        puts "Constants: #{module_info.constants}"
        puts "Module Methods: #{module_info.methods(false)}"

        module_info.constants.each do |name|
          obj = module_info.const_get(name)
          puts "#{name}: #{obj.class}"
        end

        # =========================================================
        # 2. VERSION / DEBUG
        # =========================================================
        puts "\n--- Debug Info ---"
        puts module_info.version
        puts module_info.help
        module_info._debug

        # =========================================================
        # 3. INIT
        # =========================================================
        module_info.init
        puts "Ticks: #{module_info.get_ticks}"

        # =========================================================
        # 4. WINDOW
        # =========================================================
        win = module_info::Window.new("Render Test", 800, 600)
        puts "Window: #{win}"

        # =========================================================
        # 5. AUDIO SYSTEM
        # =========================================================
        sound_files = [
          "assets/dieSound.wav",
          "assets/dingSound.wav"
        ]

        player_audio = module_info::OpenALPlayer.new(sound_files[0])
        puts "Audio Player: #{player_audio}"

        player_audio.play_sound
        player_audio.set_path(sound_files[1])
        player_audio.play_sound

        # =========================================================
        # 6. SPRITES
        # =========================================================
        sprites = {
          ae: module_info::Sprite.new("assets/ae.bmp"),
          aa: module_info::Sprite.new("assets/aa.bmp")
        }

        puts "Sprite AE: #{sprites[:ae]}"
        puts "Sprite AA: #{sprites[:aa]}"

        # =========================================================
        # 7. STATE
        # =========================================================
        state = {
          x: 400,
          y: 400,
          running: true,
          t: 0.0
        }

        # =========================================================
        # 8. INPUT CALLBACKS
        # =========================================================
        key_map = {
          "b" => proc { puts "Pressed B!" },
          "w" => proc { state[:y] -= 5 },
          "s" => proc { state[:y] += 5 },
          "a" => proc { state[:x] -= 5 },
          "d" => proc { state[:x] += 5 },
          "space" => proc { puts "Space Pressed!" }
        }

        key_map.each do |key, action|
          win.set_key_callback(module_info.get_keycode_from_name(key), action)
        end

        # =========================================================
        # 9. SCREEN COORD TEST
        # =========================================================
        sc = module_info::ScreenCoordinate.new(0, 0)

        puts "\nScreenCoordinate: #{sc}"
        puts "X: #{sc.x}"
        puts "Y: #{sc.y}"

        begin
          puts "Updating coordinates..."
        rescue NoMethodError
          puts "Setters not implemented in C yet!"
        end

        # =========================================================
        # 10. HELPERS (RENDER LOGIC)
        # =========================================================
        def draw_scene(win, sprites, state)
          # background color
          r = (Math.sin(state[:t]).abs * 255).to_i
          g = (Math.cos(state[:t]).abs * 255).to_i

          win.clear(r, g, 150, 255)

          # player sprite
          sprites[:aa].set_location(state[:x], state[:y])
          win.draw_sprite(sprites[:aa], 5, true)

          # static sprite 1
          sprites[:ae].set_location(0, 0)
          win.draw_sprite(sprites[:ae])

          # static sprite 2
          sprites[:ae].set_location(500, 500)
          win.draw_sprite(sprites[:ae], 5, true)

          win.present

          state[:t] += ((Math::PI / 4.0) / 60.0)
        end

        # =========================================================
        # 11. MAIN LOOP
        # =========================================================
        while state[:running]
          state[:running] = false unless win.poll_events
          draw_scene(win, sprites, state)
          sleep(0.016)
        end

        # =========================================================
        # 12. SHUTDOWN
        # =========================================================
        module_info.quit
    end
end


# 1. Basic Module and Version Tests
puts "--- Testing RubySDL2 Module ---"
puts RubySDL2.help
puts RubySDL2.version
puts "Ticks: #{RubySDL2.get_ticks}"

# 2. Initialization
begin
  RubySDL2.init
  puts "SDL2 Initialized successfully."
rescue => e
  puts "Failed to initialize SDL2: #{e.message}"
  exit
end

# 3. ScreenCoordinate Test
puts "\n--- Testing ScreenCoordinate ---"
coord = RubySDL2::ScreenCoordinate.new(100, 200)
puts "Coord X: #{coord.x}, Y: #{coord.y}"

# 4. Sprite Test (Requires a 'test.bmp' in the same folder)
puts "\n--- Testing Sprite ---"
begin
  # Creating a sprite at x:50, y:50
  # Note: Ensure you have a small BMP file named 'test.bmp'
  sprite = RubySDL2::Sprite.new("test.bmp", 50, 50)
  puts "Sprite loaded successfully."
rescue => e
  puts "Sprite Load Error (expected if test.bmp is missing): #{e.message}"
end

# 5. Window and Game Loop Test
puts "\n--- Opening Window (Close the window to end test) ---"
win = RubySDL2::Window.new("Ruby SDL2 Test", 800, 600)

# Define a key callback
# We use a keycode (97 is 'a' in ASCII/SDLK)
win.set_key_callback(97, proc { |key| puts "Key 'A' pressed! (Code: #{key})" })

running = true
angle = 0

while running
  # 1. Poll for Close button or Key events
  running = false unless win.poll_events

  # 2. Logic (Move sprite in a circle if it exists)
  if sprite
    angle += 0.05
    new_x = 400 + (Math.cos(angle) * 100).to_i
    new_y = 300 + (Math.sin(angle) * 100).to_i
    sprite.set_location(new_x, new_y)
  end

  # 3. Render
  win.clear(20, 20, 40, 255) # Dark blue background

  if sprite
    # Draw sprite with scale 2 and a red bounding box enabled
    win.draw_sprite(sprite, 2, true)
  end

  win.present

  sleep(0.016) # Roughly 60 FPS
end

puts "One more test for good measure"
t = Tester.new
t.test_main

RubySDL2.quit
puts "Test complete."