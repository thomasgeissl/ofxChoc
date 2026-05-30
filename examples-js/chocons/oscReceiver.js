// Port of oscReceiveExample — listen on port 12345
// Pair with oscSender.js or the C++ oscSenderExample.
//
//   ./ofJsRuntime -f ../examples-js/chocons/oscReceiver.js --watch
//
// Requires: chocons/ofxOscBindings.dylib next to this script (see examples-js/README.md)

var PORT = 12345;
var NUM_MSG_STRINGS = 20;

var receiver;
var receivedImage;

var currentMsgString = 0;
var msgStrings = [];
var timers = [];
var mouseXf = 0;
var mouseYf = 0;
var mouseButtonInt = 0;
var mouseButtonState = "";

function setup() {
    if (typeof ofx === "undefined" || !ofx.osc) {
        console.error("ofxOsc chocon not loaded — build chocons/ofxOsc and copy ofxOscBindings.dylib to chocons/");
        return;
    }
    receiver = new ofx.osc.Receiver();
    receivedImage = new of.Image();
    receiver.setup(PORT);
    of.setWindowTitle("oscReceiveExample");
    of.setFrameRate(60);
    console.log("listening for osc messages on port " + PORT);

    for (var i = 0; i < NUM_MSG_STRINGS; i++) {
        msgStrings.push("");
        timers.push(0);
    }
}

function update() {
    if (!receiver) return;
    var now = of.getElapsedTimef();

    for (var i = 0; i < NUM_MSG_STRINGS; i++) {
        if (timers[i] < now) msgStrings[i] = "";
    }

    while (receiver.hasWaitingMessages()) {
        var m = receiver.getNextMessage();
        if (!m) continue;

        if (m.address === "/mouse/position") {
            mouseXf = m.args[0];
            mouseYf = m.args[1];
        } else if (m.address === "/mouse/button") {
            mouseButtonInt = m.args[0];
            mouseButtonState = m.args[1];
        } else if (m.address === "/image") {
            var blob = m.args[0];
            if (ofx.osc.isBlob(blob) && receivedImage.loadFromBase64(blob.base64)) {
                console.log("received /image blob, size: " + blob.size);
            } else {
                console.log("received /image but failed to decode blob");
            }
        } else {
            var msgString = m.address + ":";
            for (var j = 0; j < m.args.length; j++) {
                var a = m.args[j];
                msgString += " ";
                if (ofx.osc.isBlob(a)) {
                    msgString += "blob:" + a.size;
                } else {
                    msgString += typeof a + ":" + a;
                }
            }
            msgStrings[currentMsgString] = msgString;
            timers[currentMsgString] = now + 5.0;
            currentMsgString = (currentMsgString + 1) % NUM_MSG_STRINGS;
            msgStrings[currentMsgString] = "";
        }
    }
}

function draw() {
    of.background(100, 20, 20);

    if (receivedImage && receivedImage.isAllocated() && receivedImage.getWidth() > 0) {
        of.setColor(255);
        receivedImage.draw(
            of.getWidth() / 2 - receivedImage.getWidth() / 2,
            of.getHeight() / 2 - receivedImage.getHeight() / 2
        );
    }

    of.setColor(255);
    for (var i = 0; i < NUM_MSG_STRINGS; i++) {
        if (msgStrings[i]) of.drawBitmapString(msgStrings[i], 10, 40 + 15 * i);
    }

    of.drawBitmapString("listening for osc messages on port " + PORT, 10, 20);

    var cx = mouseXf * of.getWidth();
    var cy = mouseYf * of.getHeight();
    if (mouseButtonInt === 0) {
        of.setColor(255);
    } else {
        of.setColor(250, 128, 114);
    }
    of.drawCircle(cx, cy, 20);
    of.setColor(255);
    of.drawBitmapString(mouseButtonState, cx + 24, cy + 4);
}
