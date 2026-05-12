import time
import python_sdl2

def test_reflection():
    print("--- Testing Reflection System ---")
    # PyScreenCoordinate_init requires (x, y)
    coord = python_sdl2.ScreenCoordinate(0, 0)

    # Test our generated reflection setters
    coord.set_x(420)
    coord.set_y(240)

    # Test our generated reflection getters
    x = coord.get_x()
    y = coord.get_y()

    print(f"ScreenCoordinate values: x={x}, y={y}")
    assert x == 420, "Reflection get_x/set_x failed!"
    assert y == 240, "Reflection get_y/set_y failed!"
    print("Reflection system working perfectly!\n")
    return coord

def main():
    print(f"Module Version:\n{python_sdl2.version()}")
    print("Initializing SDL2...")

    # 1. Initialize the backend
    python_sdl2.init()

    try:
        # 2. Test the Reflection Coordinate System
        coord = test_reflection()

        # 3. Create a Window object (PySDLWindow_init requires title, w, h)
        window = python_sdl2.Window("Python SDL2 Test", 800, 600)

        # 4. Set up a Key Callback
        def on_key_press(key_code):
            print(f"Callback Triggered! Key Pressed: {key_code}")

        # Get the keycode for the Spacebar using your helper method
        space_key = python_sdl2.get_keycode_from_name("Space")
        window.set_key_callback(space_key, on_key_press)

        # 5. Create and position a Sprite (PySprite_init requires a BMP path)
        # NOTE: You must have a 'test.bmp' in your directory for this to succeed!
        try:
            sprite = python_sdl2.Sprite("test.bmp")
            # Using our reflected coordinate system to drive the sprite's location
            sprite.set_location(coord.get_x(), coord.get_y())
        except python_sdl2.ArgumentError as e:
            print("Skipping sprite draw: Please provide a valid 'test.bmp'")
            sprite = None

        # 6. Test Audio (PyOpenALPlayer_init requires a WAV path)
        try:
            player = python_sdl2.OpenALPlayer("test_audio.wav")
            player.play_sound()
        except python_sdl2.OpenALError as e:
            print(f"Skipping audio: Please provide a valid 'test_audio.wav' ({e})")

        # 7. Main Game Loop
        print("\nStarting main loop. Press Spacebar to test callback, or close window to exit.")
        start_ticks = python_sdl2.get_ticks()

        running = True
        # Run for 10 seconds or until the user closes the window
        while running and (python_sdl2.get_ticks() - start_ticks < 10000):
            # poll_events returns False on SDL_QUIT
            running = window.poll_events()

            # Rendering pipeline
            # PySDLWindow_clear requires (r, g, b, a)
            window.clear(50, 50, 50, 255)

            # PySDLWindow_draw_sprite can take an optional scale and bounding box
            window.draw_sprite(sprite, 2, True)

            window.present()

            # Tiny sleep to avoid pegging the CPU to 100%
            time.sleep(0.016) # ~60fps

        print("Main loop finished.")

    except python_sdl2.SDLError as e:
        print(f"SDL Error occurred: {e}")
    except python_sdl2.ArgumentError as e:
        print(f"Argument Error: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
    finally:
        # 8. Clean up
        print("Quitting SDL2...")
        python_sdl2.quit()

if __name__ == "__main__":
    main()