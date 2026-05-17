// Port of openFrameworks examples/sound/audioInputExample
//
// Captures microphone input, draws the left and right channel waveforms,
// and displays a volume circle + smoothed-volume history graph.
//
// How it works:
//   The C++ SoundStreamWrapper captures audio on a dedicated thread and
//   double-buffers the data.  JS polls getInputSamples() / getInputRMS()
//   each frame from the main thread — no audio callback in JS is needed.
//
// Controls:
//   s — start / resume stream
//   e — stop / pause stream
//
// Run:
//   ./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime \
//     -f ../examples-js/sound/audio-input.js

// declare all display state before the stream so draw() never sees undefined
// even if the SoundStream constructor throws on a stale binary
var numCh     = 1;   // mono mic
var left      = [];
var right     = [];

var VH_SIZE   = 400;
var volHistory = [];
for (var i = 0; i < VH_SIZE; i++) volHistory.push(0);

var smoothedVol = 0;
var scaledVol   = 0;
var bufferCount = 0;
var drawCount   = 0;

var soundStream = new of.SoundStream();

function setup() {
    of.setWindowShape(1024, 640);
    of.setFrameRate(60);

    soundStream.setupInput(44100, 512, numCh);

    console.log("Audio input ready — 's' to start, 'e' to pause");
}

function update() {
    var samples   = soundStream.getInputSamples();
    var numFrames = numCh > 0 ? Math.floor(samples.length / numCh) : 0;

    // split interleaved buffer into per-channel arrays
    // (mono input → left and right are copies of channel 0, scaled like the original)
    left  = [];
    right = [];
    for (var i = 0; i < numFrames; i++) {
        var s = samples[i * numCh] * 0.5;
        left.push(s);
        right.push(s);
    }

    // smooth RMS into a volume history
    scaledVol = of.map(soundStream.getInputRMS(), 0, 0.17, 0, 1, true);
    volHistory.push(scaledVol);
    if (volHistory.length >= VH_SIZE) volHistory.shift();

    bufferCount++;
    of.setWindowTitle("audio-input  |  " + Math.round(of.getFrameRate()) + " fps");
}

function draw() {
    of.background(54, 54, 54);

    of.setColor(225);
    of.drawBitmapString("AUDIO INPUT EXAMPLE", 32, 32);
    of.drawBitmapString("press 's' to unpause the audio\n'e' to pause the audio", 31, 92);

    of.noFill();

    // ── left channel ─────────────────────────────────────────────────────────
    of.pushStyle();
    of.pushMatrix();
    of.translate(32, 170);

    of.setColor(225);
    of.drawBitmapString("Left Channel", 4, 18);
    of.setLineWidth(1);
    of.drawRectangle(0, 0, 512, 200);

    if (left.length > 0) {
        of.setColor(245, 58, 135);
        of.setLineWidth(3);
        of.beginShape();
        for (var i = 0; i < left.length; i++)
            of.vertex(i * 2, 100 - left[i] * 180);
        of.endShape();
    }

    of.popMatrix();
    of.popStyle();

    // ── right channel ────────────────────────────────────────────────────────
    of.pushStyle();
    of.pushMatrix();
    of.translate(32, 370);

    of.setColor(225);
    of.drawBitmapString("Right Channel", 4, 18);
    of.setLineWidth(1);
    of.drawRectangle(0, 0, 512, 200);

    if (right.length > 0) {
        of.setColor(245, 58, 135);
        of.setLineWidth(3);
        of.beginShape();
        for (var i = 0; i < right.length; i++)
            of.vertex(i * 2, 100 - right[i] * 180);
        of.endShape();
    }

    of.popMatrix();
    of.popStyle();

    // ── volume circle + history graph ─────────────────────────────────────────
    of.pushStyle();
    of.pushMatrix();
    of.translate(565, 170);

    of.setColor(225);
    of.drawBitmapString(
        "Scaled average vol (0-100): " + Math.round(scaledVol * 100),
        4, 18);
    of.noFill();
    of.drawRectangle(0, 0, 400, 400);

    of.setColor(245, 58, 135);
    of.fill();
    of.drawCircle(200, 200, scaledVol * 190);

    // history as a filled polygon (matches original ofBeginShape / ofVertex loop)
    of.beginShape();
    for (var i = 0; i < volHistory.length; i++) {
        if (i === 0) of.vertex(i, 400);
        of.vertex(i, 400 - volHistory[i] * 70);
        if (i === volHistory.length - 1) of.vertex(i, 400);
    }
    of.endShape();

    of.popMatrix();
    of.popStyle();

    drawCount++;
    of.setColor(225);
    of.drawBitmapString(
        "update() calls: " + bufferCount + "\ndraw() calls: " + drawCount,
        32, 609);
}

function keyPressed(key) {
    if (key === 115) soundStream.start();  // s
    if (key === 101) soundStream.stop();   // e
}

setup();
