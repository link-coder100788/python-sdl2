import python_sdl2
import time
import math
import inspect

print(dir(python_sdl2))
print(vars(python_sdl2))
for name, obj in inspect.getmembers(python_sdl2):
    print(name, type(obj))

print("Version info:")
print(python_sdl2.version())
print(python_sdl2.help())
python_sdl2._debug()

python_sdl2.init()

print(python_sdl2.get_ticks())

win = python_sdl2.Window("Render Test", 800, 600)
print(win)

#python_sdl2.play_sound("assets/dieSound.wav")

player = python_sdl2.OpenALPlayer("assets/dieSound.wav")
print(player)
#player.play_sound()
player.set_path("assets/dingSound.wav")
#player.play_sound()

running = True
t = 0

sprite = python_sdl2.Sprite("assets/ae.bmp")
print(sprite)

player = python_sdl2.Sprite("assets/aa.bmp")

x = 400
y = 400


def on_b(key):
    print("Pressed B!")

def on_w(key):
    global y
    y -= 5
def on_s(key):
    global y
    y += 5
def on_a(key):
    global x
    x -= 5
def on_d(key):
    global x
    x += 5

b_code = python_sdl2.get_keycode_from_name("b")
w_code = python_sdl2.get_keycode_from_name("w")
s_code = python_sdl2.get_keycode_from_name("s")
a_code = python_sdl2.get_keycode_from_name("a")
d_code = python_sdl2.get_keycode_from_name("d")

win.set_key_callback(b_code, on_b)
win.set_key_callback(w_code, on_w)
win.set_key_callback(s_code, on_s)
win.set_key_callback(a_code, on_a)
win.set_key_callback(d_code, on_d)

sc = python_sdl2.ScreenCoordinate(0, 0)
print(sc)
print(sc.get_x())
print(sc.get_y())
sc.set_x(100)
sc.set_y(100)
print(sc)
print(sc.get_x())
print(sc.get_y())

print(sc.get_x.__doc__)
print(sc.get_y.__doc__)
print(sc.set_x.__doc__)
print(sc.set_y.__doc__)

while running:
    running = win.poll_events()
    win.clear(int(abs(math.sin(t)) * 255), int(abs(math.cos(t)) * 255), 150, 255)
    player.set_location(x, y)
    win.draw_sprite(player, 5, True)
    sprite.set_location(0, 0)
    win.draw_sprite(sprite)
    sprite.set_location(500, 500)
    win.draw_sprite(sprite, 5, True)
    win.present()
    t += ((math.pi / 4) / 60)
    time.sleep(0.016)

python_sdl2.quit()