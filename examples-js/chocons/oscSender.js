// Port of oscSenderExample — send OSC to localhost:12345
// Run with oscReceiver.js in another ofJsRuntime instance (or the C++ oscReceiveExample).
//
//   ./ofJsRuntime -f ../examples-js/chocons/oscSender.js --watch
//
// Requires: chocons/ofxOscBindings.dylib next to this script (see examples-js/README.md)
//
// Controls:
//   mouse move  → /mouse/position (normalized x, y)
//   mouse down/up → /mouse/button
//   A           → /test 1 3.5 hello <time>
//   I           → /image blob (PNG from data/of-logo.png)

var HOST = "localhost";
var PORT = 12345;

var sender;
var img;
var logoPath = __dirname + "/data/of-logo.png";

function setup() {
    if (typeof ofx === "undefined" || !ofx.osc) {
        console.error("ofxOsc chocon not loaded — build chocons/ofxOsc and copy ofxOscBindings.dylib to chocons/");
        return;
    }
    sender = new ofx.osc.Sender();
    img = new of.Image();
    sender.setup(HOST, PORT);
    of.setWindowTitle("oscSenderExample");
    of.setFrameRate(60);
    img.load(logoPath);
}

function update() {
    if (!sender) return;
    var mx = of.map(of.getMouseX(), 0, of.getWidth(), 0, 1, true);
    var my = of.map(of.getMouseY(), 0, of.getHeight(), 0, 1, true);
    sender.send("/mouse/position", mx, my);
}

function draw() {
    of.background(255, 180, 180);

    if (img && img.isAllocated()) {
        of.setColor(255);
        img.draw(
            of.getWidth() / 2 - img.getWidth() / 2,
            of.getHeight() / 2 - img.getHeight() / 2
        );
    }

    of.setColor(0);
    var buf = "sending osc messages to: " + HOST + " " + PORT;
    buf += "\npress A to send osc message [/test 1 3.5 hello <time>]";
    buf += "\npress I to send a (small) image as a osc blob to [/image]";
    of.drawBitmapString(buf, 10, 20);
}

function keyPressed(key) {
    if (!sender) return;
    if (key === 97 || key === 65) {
        sender.send("/test", 1, 3.5, "hello", of.getElapsedTimef());
    }
    if (key === 105 || key === 73) {
        var size = sender.sendBlobFromFile("/image", logoPath);
        if (size > 0) {
            img.load(logoPath);
            console.log("sending image with size: " + size);
        } else {
            console.log("failed to send /image blob — is " + logoPath + " present?");
        }
    }
}

function mousePressed(x, y, button) {
    if (!sender) return;
    sender.send("/mouse/button", 1, "down");
}

function mouseReleased(x, y, button) {
    if (!sender) return;
    sender.send("/mouse/button", 0, "up");
}
