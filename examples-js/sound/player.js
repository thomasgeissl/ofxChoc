// Port of openFrameworks examples/sound/soundPlayerExample
//
// Three sound files displayed in three columns.
//   Left   column (synth.wav)  — click to play; mouse Y → speed, X → pan
//   Middle column (1085.mp3)   — click to play; drag up/down to scrub speed
//   Right  column (Violet.mp3) — click to play (multiplay); Y → speed, X → pan
//
// The sound files are read from the OF example's data folder.  Adjust the
// path below if you keep the files elsewhere.
//
// Run:
//   ./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime \
//     -f ../examples-js/sound/player.js

var DATA = "/Users/thomas/projects/of_v0.12.1_osx_release/examples/sound/soundPlayerExample/bin/data/";

var synth  = new of.SoundPlayer();
var beats  = new of.SoundPlayer();
var vocals = new of.SoundPlayer();

var font = new of.Font();

// track values we can't read back from the player
var synthSpeed  = 1.0, synthPan  = 0.0;
var beatsSpeed  = 1.0;
var vocalsSpeed = 1.0, vocalsPan = 0.0;

function setup() {
    of.setWindowShape(1024, 600);
    of.setFrameRate(60);

    synth.load(DATA + "sounds/synth.wav");
    beats.load(DATA + "sounds/1085.mp3");
    vocals.load(DATA + "sounds/Violet.mp3");

    synth.setVolume(0.75);
    beats.setVolume(0.75);
    vocals.setVolume(0.5);

    beats.setMultiPlay(false);
    vocals.setMultiPlay(true);

    font.load(DATA + "Sudbury_Basin_3D.ttf", 32);

    console.log("Sound player ready");
}

function update() {
    of.setWindowTitle("sound-player  |  " + Math.round(of.getFrameRate()) + " fps");
}

function draw() {
    var w   = of.getWidth();
    var h   = of.getHeight();
    var col = w / 3.0;

    // column backgrounds
    of.fill();
    of.setColor(238, 238, 238);
    of.drawRectangle(0,       0, col, h);
    of.setColor(255, 255, 255);
    of.drawRectangle(col,     0, col, h);
    of.setColor(221, 221, 221);
    of.drawRectangle(col * 2, 0, col, h);

    // ── synth column ──────────────────────────────────────────────────────────
    of.setColor(synth.isPlaying() ? 255 : 0, 0, 0);
    font.drawString("synth !!", 50, 60);

    of.setColor(0, 0, 0);
    of.drawBitmapString(
        "click to play\n" +
        "pct done: " + synth.getPosition().toFixed(2) + "\n" +
        "speed: "    + synthSpeed.toFixed(2) + "\n" +
        "pan: "      + synthPan.toFixed(2),
        50, h - 70);

    // ── beats column ─────────────────────────────────────────────────────────
    of.setColor(beats.isPlaying() ? 255 : 0, 0, 0);
    font.drawString("beats !!", col + 50, 60);

    of.setColor(0, 0, 0);
    of.drawBitmapString(
        "click and drag\n" +
        "pct done: " + beats.getPosition().toFixed(2) + "\n" +
        "speed: "    + beatsSpeed.toFixed(2),
        col + 50, h - 70);

    // ── vocals column ─────────────────────────────────────────────────────────
    of.setColor(vocals.isPlaying() ? 255 : 0, 0, 0);
    font.drawString("vocals !!", col * 2 + 50, 60);

    of.setColor(0, 0, 0);
    of.drawBitmapString(
        "click to play (multiplay)\n" +
        "pct done: " + vocals.getPosition().toFixed(2) + "\n" +
        "speed: "    + vocalsSpeed.toFixed(2),
        col * 2 + 50, h - 70);
}

function mouseDragged(x, y, button) {
    var col = of.getWidth() / 3.0;
    if (x >= col && x < col * 2) {
        beatsSpeed = 0.5 + ((of.getHeight() - y) / of.getHeight()) * 1.0;
        beats.setSpeed(beatsSpeed);
    }
}

function mousePressed(x, y, button) {
    var w   = of.getWidth();
    var h   = of.getHeight();
    var col = w / 3.0;

    if (x < col) {
        synthSpeed = 0.1 + ((h - y) / h) * 10;
        synthPan   = of.map(x, 0, col, -1, 1, true);
        synth.setSpeed(synthSpeed);
        synth.setPan(synthPan);
        synth.play();
    } else if (x < col * 2) {
        beats.play();
    } else {
        vocalsSpeed = 0.1 + ((h - y) / h) * 3;
        vocalsPan   = of.map(x, col * 2, col * 3, -1, 1, true);
        vocals.setSpeed(vocalsSpeed);
        vocals.setPan(vocalsPan);
        vocals.play();
    }
}

setup();
