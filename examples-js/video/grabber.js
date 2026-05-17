// Port of openFrameworks examples/video/videoGrabberExample
//
// Shows the live webcam feed alongside a colour-inverted copy.
// Inversion is done entirely on the GPU with a two-line GLSL fragment shader
// (no custom C++ helpers — just of.Shader + of.VideoGrabber).
//
// Controls:
//   s / S — open video settings panel
//
// Run:
//   ./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime \
//     -f ../examples-js/video/grabber.js

var camW = 640;
var camH = 480;

var grabber     = new of.VideoGrabber();
var invertShader = new of.Shader();

function setup() {
    of.setFrameRate(60);

    var devices = grabber.listDevices();
    for (var i = 0; i < devices.length; i++)
        console.log(devices[i].id + ": " + devices[i].name);

    grabber.setDeviceID(0);
    grabber.setDesiredFrameRate(30);
    grabber.setup(camW, camH);

    var base = __dirname + "/shaders/invert";
    var ok = invertShader.load(base + ".vert", base + ".frag");
    if (!ok) console.log("ERROR: could not load invert shader");
}

function update() {
    grabber.update();
    of.setWindowTitle("video-grabber  |  " + Math.round(of.getFrameRate()) + " fps");
}

function draw() {
    of.background(100, 100, 100);
    of.setColor(255, 255, 255, 255);

    // Left: raw camera
    grabber.draw(20, 20);

    // Right: invert every RGB channel on the GPU
    invertShader.begin();
    grabber.draw(20 + camW, 20);
    invertShader.end();

    of.setColor(220, 220, 220, 255);
    of.drawBitmapString("live", 30, 18);
    of.drawBitmapString("inverted (shader)", 30 + camW, 18);
    of.drawBitmapString("press s for video settings", 20, camH + 40);
}

function keyPressed(key) {
    if (key === 115 || key === 83)  // 's' / 'S'
        grabber.videoSettings();
}

setup();
