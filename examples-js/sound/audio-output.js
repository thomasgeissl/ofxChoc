// Port of openFrameworks examples/sound/audioOutputExample
//
// Synthesises a sine wave (or noise) and draws the left/right waveforms.
//
// How it works:
//   JS computes a block of PCM samples in update() and pushes them into the
//   C++ SoundStreamWrapper's ring buffer.  The audio thread drains the ring
//   buffer each callback.  JS must push enough samples per frame to keep the
//   ring full (~sampleRate × numChannels / frameRate per update call).
//
// Controls:
//   Mouse X     — pan  (left ↔ right)
//   Mouse Y     — frequency (bottom=0 Hz, top=2000 Hz)
//   Click       — hold for noise, release for sine
//   - / _       — decrease volume
//   + / =       — increase volume
//   s           — start / resume stream
//   e           — stop  / pause  stream
//
// Run:
//   ./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime \
//     -f ../examples-js/sound/audio-output.js

var SAMPLE_RATE = 44100;
var NUM_CH      = 2;
var BUFFER_SIZE = 512;
var TWO_PI      = Math.PI * 2;

// declare display buffers first so draw() never sees them as undefined
// even if the SoundStream constructor throws on a stale binary
var lAudio = [];
var rAudio = [];

var phase             = 0;
var phaseAdder        = 0;
var phaseAdderTarget  = 0;
var volume            = 0.1;
var pan               = 0.5;
var targetFrequency   = 0;
var bNoise            = false;

var soundStream = new of.SoundStream();

function setup() {
    of.setWindowShape(1024, 600);
    of.setFrameRate(60);

    soundStream.setupOutput(SAMPLE_RATE, BUFFER_SIZE, NUM_CH);

    console.log("Audio output ready — move mouse: X=pan Y=freq | click=noise | -/+=volume | s/e=start/stop");
}

function update() {
    // Synthesise enough samples to cover at least one frame of audio, plus
    // one extra buffer's worth of headroom to absorb timing jitter.
    var samplesNeeded = Math.ceil(SAMPLE_RATE * NUM_CH / 60) + BUFFER_SIZE * NUM_CH;
    var leftScale     = 1 - pan;
    var rightScale    = pan;
    var out           = [];

    // keep phase in [0, 2π] to avoid precision loss at large values
    while (phase > TWO_PI) phase -= TWO_PI;

    for (var i = 0; i < samplesNeeded; i += NUM_CH) {
        var lSample, rSample;
        if (bNoise) {
            var noise = (Math.random() * 2 - 1) * volume;
            lSample   = noise * leftScale;
            rSample   = noise * rightScale;
        } else {
            phaseAdder = 0.95 * phaseAdder + 0.05 * phaseAdderTarget;
            phase     += phaseAdder;
            var s      = Math.sin(phase) * volume;
            lSample    = s * leftScale;
            rSample    = s * rightScale;
        }
        out.push(lSample);
        out.push(rSample);
    }
    soundStream.pushOutputSamples(out);

    // grab the last played buffer for visualization
    var last      = soundStream.getLastOutputSamples();
    var numFrames = Math.floor(last.length / NUM_CH);
    lAudio = [];
    rAudio = [];
    for (var j = 0; j < numFrames; j++) {
        lAudio.push(last[j * NUM_CH]);
        rAudio.push(last[j * NUM_CH + 1]);
    }

    of.setWindowTitle("audio-output  |  " + Math.round(of.getFrameRate()) + " fps");
}

function draw() {
    of.background(34, 34, 34);

    of.setColor(225);
    of.drawBitmapString("AUDIO OUTPUT EXAMPLE", 32, 32);
    of.drawBitmapString("press 's' to unpause the audio\npress 'e' to pause the audio", 31, 92);

    of.noFill();

    // ── left channel ─────────────────────────────────────────────────────────
    of.pushStyle();
    of.pushMatrix();
    of.translate(32, 150);

    of.setColor(225);
    of.drawBitmapString("Left Channel", 4, 18);
    of.setLineWidth(1);
    of.drawRectangle(0, 0, 900, 200);

    if (lAudio.length > 0) {
        of.setColor(245, 58, 135);
        of.setLineWidth(3);
        of.beginShape();
        for (var i = 0; i < lAudio.length; i++) {
            var x = of.map(i, 0, lAudio.length, 0, 900, true);
            of.vertex(x, 100 - lAudio[i] * 180);
        }
        of.endShape();
    }

    of.popMatrix();
    of.popStyle();

    // ── right channel ────────────────────────────────────────────────────────
    of.pushStyle();
    of.pushMatrix();
    of.translate(32, 350);

    of.setColor(225);
    of.drawBitmapString("Right Channel", 4, 18);
    of.setLineWidth(1);
    of.drawRectangle(0, 0, 900, 200);

    if (rAudio.length > 0) {
        of.setColor(245, 58, 135);
        of.setLineWidth(3);
        of.beginShape();
        for (var i = 0; i < rAudio.length; i++) {
            var x = of.map(i, 0, rAudio.length, 0, 900, true);
            of.vertex(x, 100 - rAudio[i] * 180);
        }
        of.endShape();
    }

    of.popMatrix();
    of.popStyle();

    // ── HUD ──────────────────────────────────────────────────────────────────
    var synthType = bNoise
        ? "noise"
        : "sine wave (" + Math.round(targetFrequency) + " hz) — modify with mouse y";
    of.setColor(225);
    of.drawBitmapString(
        "volume: (" + volume.toFixed(2) + ") modify with -/+ keys\n" +
        "pan: ("    + pan.toFixed(2)    + ") modify with mouse x\n" +
        "synthesis: " + synthType,
        32, 579);
}

function mouseMoved(x, y) {
    pan             = x / of.getWidth();
    var heightPct   = (of.getHeight() - y) / of.getHeight();
    targetFrequency = 2000 * heightPct;
    phaseAdderTarget = (targetFrequency / SAMPLE_RATE) * TWO_PI;
}

function mouseDragged(x, y, button) {
    pan = x / of.getWidth();
}

function mousePressed(x, y, button)  { bNoise = true;  }
function mouseReleased(x, y, button) { bNoise = false; }

function keyPressed(key) {
    if (key === 45 || key === 95) { volume = Math.max(0,    volume - 0.05); }  // - _
    if (key === 43 || key === 61) { volume = Math.min(1,    volume + 0.05); }  // + =
    if (key === 115) soundStream.start();  // s
    if (key === 101) soundStream.stop();   // e
}

setup();
