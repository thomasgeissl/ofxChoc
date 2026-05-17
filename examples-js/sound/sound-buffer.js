// Port of openFrameworks examples/sound/soundBufferExample
//
// Synthesises a chord: three detuned sine oscillators, each amplitude-
// modulated by its own LFO at slightly different rates.  Draws the left-
// channel waveform, with line width proportional to the RMS amplitude.
//
// How it works:
//   Same push/pull architecture as audio-output.js.  JS computes samples in
//   update() and pushes them into the C++ ring buffer; the audio thread drains
//   them.  getLastOutputSamples() and getLastOutputRMS() are polled each frame
//   for the waveform visualization.
//
// No controls — just listen and watch.
//
// Run:
//   ./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime \
//     -f ../examples-js/sound/sound-buffer.js

var SAMPLE_RATE = 44100;
var NUM_CH      = 2;
var BUFFER_SIZE = 512;
var TWO_PI      = Math.PI * 2;

var wavePhase  = 0;
var pulsePhase = 0;

var soundStream = new of.SoundStream();

function setup() {
    of.setWindowShape(1024, 600);
    of.setFrameRate(60);

    soundStream.setupOutput(SAMPLE_RATE, BUFFER_SIZE, NUM_CH);

    console.log("Sound buffer ready — listen to the evolving chord");
}

function update() {
    // base frequency and per-sample phase increments
    var frequency = 172.5;
    var waveStep  = (frequency / SAMPLE_RATE) * TWO_PI;
    var pulseStep = (0.5      / SAMPLE_RATE) * TWO_PI;

    // compute enough samples to cover ~1 frame + one extra buffer of headroom
    var samplesNeeded = Math.ceil(SAMPLE_RATE * NUM_CH / 60) + BUFFER_SIZE * NUM_CH;
    var out = [];

    for (var i = 0; i < samplesNeeded; i += NUM_CH) {
        // three detuned oscillators
        var sLow = Math.sin(wavePhase);
        var sMid = Math.sin(wavePhase * 1.5);
        var sHi  = Math.sin(wavePhase * 2.0);

        // three independent LFOs
        sLow *= Math.sin(pulsePhase);
        sMid *= Math.sin(pulsePhase * 1.04);
        sHi  *= Math.sin(pulsePhase * 1.09);

        var full = (sLow + sMid + sHi) * 0.3;

        out.push(full);  // L
        out.push(full);  // R

        wavePhase  += waveStep;
        pulsePhase += pulseStep;
    }

    soundStream.pushOutputSamples(out);
    of.setWindowTitle("sound-buffer  |  " + Math.round(of.getFrameRate()) + " fps");
}

function draw() {
    of.background(0, 0, 0);

    var scrW      = of.getWidth();
    var scrH      = of.getHeight();
    var last      = soundStream.getLastOutputSamples();
    var numFrames = Math.floor(last.length / NUM_CH);
    var rms       = soundStream.getLastOutputRMS();

    of.setColor(255, 255, 255);
    of.setLineWidth(1 + rms * 30);
    of.noFill();

    if (numFrames > 0) {
        of.beginShape();
        for (var i = 0; i < numFrames; i++) {
            var sample = last[i * NUM_CH];  // left channel
            var x = of.map(i, 0, numFrames, 0, scrW);
            var y = of.map(sample, -1, 1, 0, scrH);
            of.vertex(x, y);
        }
        of.endShape();
    }
}

setup();
