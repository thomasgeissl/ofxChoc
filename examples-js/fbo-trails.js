// Port of openFrameworks examples/gl/fboTrailsExample
//
// Two FBOs drawn side-by-side:
//  left  — standard RGBA (8 bit per channel)
//  right — floating-point RGBA32F (higher precision, smoother fades)
//
// Controls:
//   Mouse     — paint circles
//   1 / 2 / 3 — slow / medium / fast fade
//   c         — clear both FBOs
//
// Run:
//   ./ofJsRuntime -f /path/to/fbo-trails.js

var fboW = 400;
var fboH = 400;
var GL_RGBA       = 6408;   // 0x1908
var GL_RGBA32F    = 34836;  // 0x8814  (float FBO)

var rgbaFbo      = new of.Fbo();
var rgbaFloatFbo = new of.Fbo();
var fadeAmnt     = 40;

function setup() {
    of.setFrameRate(60);

    // Allocate both FBOs
    rgbaFbo.allocate(fboW, fboH, GL_RGBA);
    rgbaFloatFbo.allocate(fboW, fboH, GL_RGBA32F);

    // Clear to transparent white so we start with no artefacts
    rgbaFbo.begin();
    of.clear(255, 255, 255, 0);
    rgbaFbo.end();

    rgbaFloatFbo.begin();
    of.clear(255, 255, 255, 0);
    rgbaFloatFbo.end();

    console.log("FBOs allocated: " + rgbaFbo.getWidth() + "x" + rgbaFbo.getHeight());
}

// Draw trails, rotating box, and mouse circle into the current FBO
function drawFboContent() {
    // Key-controlled fade speed
    fadeAmnt = 40;
    if (of.getKeyPressed(49)) fadeAmnt = 1;       // '1'
    else if (of.getKeyPressed(50)) fadeAmnt = 5;  // '2'
    else if (of.getKeyPressed(51)) fadeAmnt = 15; // '3'

    // Clear with c key
    if (of.getKeyPressed(99)) {                   // 'c'
        of.clear(255, 255, 255, 0);
        return;
    }

    // 1 — Fade: draw a translucent white rect over the whole FBO
    of.enableAlphaBlending();
    of.fill();
    of.setColor(255, 255, 255, fadeAmnt);
    of.drawRectangle(0, 0, fboW, fboH);

    // 2 — Rotating wireframe box in the centre
    of.noFill();
    of.setColor(255, 255, 255, 255);
    of.pushMatrix();
    of.translate(fboW * 0.5, fboH * 0.5, 0);
    of.rotateDeg(of.getElapsedTimef() * 30, 1, 0, 0.5);
    of.drawBox(0, 0, 0, 100);
    of.popMatrix();

    // 3 — Circle following the mouse (position mapped into the FBO)
    of.fill();
    of.drawCircle(of.getMouseX() % (fboW + 10), of.getMouseY(), 8);

    // 4 — Moving rectangle ticker
    var shiftX = Math.floor(of.getElapsedTimeMillis() / 8) % fboW;
    of.drawRectangle(shiftX, fboH - 30, 3, 30);
}

function update() {
    of.setWindowTitle("fbo-trails  |  " + Math.round(of.getFrameRate()) + " fps");
    of.enableAlphaBlending();

    rgbaFbo.begin();
    drawFboContent();
    rgbaFbo.end();

    rgbaFloatFbo.begin();
    drawFboContent();
    rgbaFloatFbo.end();
}

function draw() {
    of.setColor(255, 255, 255, 255);

    // Draw both FBOs side by side
    rgbaFbo.draw(0, 0);
    rgbaFloatFbo.draw(fboW + 10, 0);

    // Labels
    of.setColor(200, 200, 200, 255);
    of.drawBitmapString("RGBA (8-bit)", 10, 20);
    of.drawBitmapString("RGBA32F (float)", fboW + 20, 20);

    of.setColor(160, 160, 160, 255);
    var info = "Hold 1: slow fade   2: medium   3: fast   c: clear";
    of.drawBitmapString(info, 10, fboH + 20);
}

setup();
