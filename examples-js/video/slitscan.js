// Port of openFrameworks examples/video/slitscanRadialClockExample
// Original © Daniel Buzzo 2020-22
//
// Slitscan clock — one thin strip of the camera is written into an FBO "tape"
// each second.  Display modes:
//   Radial: a polar-warp shader maps the flat tape onto a clock-face ring in a
//           single GPU draw call (no per-column JS loop).
//   Flat:   the tape is stretched to fill the window.
//
// Controls:
//   r — toggle radial / flat
//   d — toggle debug overlay (camera thumbnail + status)
//   f — toggle fullscreen
//
// Run:
//   ./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime \
//     -f ../examples-js/video/slitscan.js

var camW = 1280;
var camH = 720;

var grabber     = new of.VideoGrabber();
var tape        = new of.Fbo();
var polarShader = new of.Shader();

var bRadial = true;
var bDebug  = false;

var seconds = -1;
var minutes = 0;
var hours   = 0;
var numOfSecs = 59;
var timeStr   = "00:00:00";

function pad2(n) { return n < 10 ? "0" + n : "" + n; }
function updateTimeStr() {
    timeStr = pad2(hours) + ":" + pad2(minutes) + ":" + pad2(seconds);
}

function setup() {
    of.setFrameRate(60);

    var devices = grabber.listDevices();
    for (var i = 0; i < devices.length; i++)
        console.log(devices[i].id + ": " + devices[i].name);

    grabber.setDeviceID(0);
    grabber.setDesiredFrameRate(30);
    grabber.setup(camW, camH);

    tape.allocate(camW, camH);
    tape.begin();
    of.clear(0, 0, 0, 255);
    tape.end();

    var base = __dirname + "/shaders/polar";
    var ok = polarShader.load(base + ".vert", base + ".frag");
    if (!ok) console.log("ERROR: could not load polar shader");

    console.log("Slitscan clock ready  —  r:radial  d:debug  f:fullscreen");
}

function update() {
    grabber.update();
    if (!grabber.isFrameNew()) return;

    var nowSec = of.getSeconds();
    if (nowSec === seconds) return;

    seconds = nowSec;
    minutes = of.getMinutes();
    hours   = of.getHours();
    updateTimeStr();

    // Write one squished strip of the camera into the tape FBO at the correct
    // horizontal position for this second.  No pixel loops — just GL draw.
    var stripW = Math.max(1, Math.floor(camW / numOfSecs));
    var stripX = seconds * stripW;

    tape.begin();
    of.pushMatrix();
    of.translate(stripX, 0);
    of.setColor(255, 255, 255, 255);
    grabber.draw(0, 0, stripW, camH);
    of.popMatrix();
    tape.end();

    of.setWindowTitle("slitscan  |  " + timeStr + "  |  " + Math.round(of.getFrameRate()) + " fps");
}

function draw() {
    of.background(0, 0, 0);

    var scrW = of.getWidth();
    var scrH = of.getHeight();

    of.setColor(255, 255, 255, 255);

    if (bRadial) {
        // Single GPU draw call: the polar-warp shader samples tex0 (the tape)
        // using gl_FragCoord → polar math.  No JS loop over columns needed.
        polarShader.begin();
        polarShader.setUniform2f("resolution", scrW, scrH);
        polarShader.setUniform2f("tapeSize",   camW, camH);
        tape.draw(0, 0, scrW, scrH);
        polarShader.end();
    } else {
        tape.draw(0, 0, scrW, scrH);
    }

    of.setColor(255, 255, 255, 200);
    of.drawBitmapString(timeStr, scrW - 120, scrH - 20);

    if (bDebug) {
        var thumbW = Math.floor(camW / 4);
        var thumbH = Math.floor(camH / 4);
        of.setColor(255, 255, 255, 255);
        grabber.draw(scrW - thumbW - 10, scrH - thumbH - 10, thumbW, thumbH);
        of.setColor(180, 180, 180, 255);
        of.drawBitmapString(
            "r:radial  f:fullscreen  d:debug  " + Math.round(of.getFrameRate()) + " fps",
            10, scrH - 10);
    }
}

function keyPressed(key) {
    if      (key === 114) bRadial = !bRadial;
    else if (key === 100) bDebug  = !bDebug;
    else if (key === 102) of.toggleFullscreen();
}

setup();
