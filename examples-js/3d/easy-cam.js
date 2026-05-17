// Port of openFrameworks examples/3d/easyCamExample
//
// Interactive 3D camera controlled by the mouse.
// Six coloured shapes sit at the ends of the three world axes.
//
// Controls:
//   Left drag inside  yellow circle — orbit (rotate)
//   Left drag outside yellow circle — roll
//   Right drag / scroll wheel       — zoom in / out
//   h                               — toggle help text
//   f                               — toggle fullscreen
//
// Run:
//   ./ofJsRuntime -f /path/to/3d/easy-cam.js

var cam       = new of.EasyCam();
var helpText  = true;

function draw() {
    of.background(20);

    cam.begin();
    of.enableDepthTest();

    // +X  red sphere
    of.setColor(255, 0, 0);
    of.drawSphere(100, 0, 0, 50);

    // -X  white sphere
    of.setColor(255, 255, 255);
    of.drawSphere(-100, 0, 0, 50);

    // +Y  blue box  (down in OF's Y-down convention)
    of.setColor(0, 0, 255);
    of.drawBox(0, 100, 0, 100);

    // -Y  cyan box
    of.setColor(0, 255, 255);
    of.drawBox(0, -100, 0, 100);

    // +Z  yellow box  (toward viewer)
    of.setColor(255, 255, 0);
    of.drawBox(0, 0, 100, 100);

    // -Z  magenta box
    of.setColor(255, 0, 255);
    of.drawBox(0, 0, -100, 100);

    of.disableDepthTest();
    cam.end();

    // ── Interaction guide circle ──────────────────────────────────────────
    var w = of.getWidth();
    var h = of.getHeight();
    of.pushStyle();
    of.noFill();
    of.setLineWidth(3);
    of.setColor(255, 255, 0);
    of.drawCircle(w * 0.5, h * 0.5, Math.min(w, h) * 0.5);
    of.popStyle();

    // ── HUD ───────────────────────────────────────────────────────────────
    if (helpText) {
        of.setColor(255);
        of.drawBitmapString(
            "FPS: " + Math.round(of.getFrameRate()) + "\n\n" +
            "Drag inside  circle — orbit\n" +
            "Drag outside circle — roll\n" +
            "Right drag / scroll — zoom\n" +
            "h: toggle help\n" +
            "f: fullscreen",
            20, 20
        );
    }
}

function keyPressed(key) {
    if (key === 104 || key === 72) helpText = !helpText;   // h / H
    if (key === 102 || key === 70) of.toggleFullscreen();  // f / F
}
