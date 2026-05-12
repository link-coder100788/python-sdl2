import dev.linkcoder100788.java_sdl2.JavaSDL2;
import dev.linkcoder100788.java_sdl2.JavaSDL2.Window;
import dev.linkcoder100788.java_sdl2.JavaSDL2.Sprite;
import dev.linkcoder100788.java_sdl2.JavaSDL2.OpenALPlayer;
import dev.linkcoder100788.java_sdl2.JavaSDL2.ScreenCoordinate;
import java.util.Random;

public class Test {
    public static void main(String[] args) {
        JavaSDL2.version();
        JavaSDL2.help();
        JavaSDL2._debug();
        System.out.println("GoodBye!");
        JavaSDL2.init();
        JavaSDL2.playSound("assets/dingSound.wav");
        Window w = new Window("Hello, World!", 600, 600);
        Sprite s = new Sprite("assets/aa.bmp");
        OpenALPlayer a = new OpenALPlayer("assets/dieSound.wav");
        ScreenCoordinate c = new ScreenCoordinate(Integer.MAX_VALUE, 300);
        a.playSound();
        a.setPath("assets/dingSound.wav");
        a.playSound();
        Random r = new Random();
        System.out.println("Window: " + w.toString());
        System.out.println("Sprite: " + s.toString());
        System.out.println("OpenALPlayer: " + a.toString());
        System.out.println("ScreenCoordinate: " + c.toString());
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
        w.onKey(JavaSDL2.getKeycodeFromName("d"), key -> {
            s.setX(r.nextInt(600));
        });
        s.setY(r.nextInt(600));
        boolean running = true;
        double t = 0;
        while (running) {
            running = w.pollEvent();
            w.clear((int)(Math.abs(Math.sin(t)) * 255), (int)(Math.abs(Math.cos(t)) * 255), 150, 255);
            w.drawSpriteScaledBounded(s, 5, true);
            w.present();
            t += ((Math.PI / 4.0f) / 60.0f);
            try {
                Thread.sleep(16);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        w.destroy();
        s.destroy();
        a.destroy();
    }
}