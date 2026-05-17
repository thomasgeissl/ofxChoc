// Port of openFrameworks examples/sound/soundPlayerFFTExample
//
// A circle bounces around the screen and plays a different sound on each wall
// hit.  Volume of each hit is proportional to the ball's speed at impact.
// Drag the mouse to fling the ball.
//
// Note: ofSoundGetSpectrum() is not available in the JS runtime, so the FFT
// bar visualisation is replaced with a rolling velocity history graph drawn
// in the same region at the bottom of the screen.
//
// Sound files are read from the OF example's data folder.  Adjust the path
// below if you keep the files elsewhere.
//
// Run:
//   ./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime \
//     -f ../examples-js/sound/bouncing.js

var DATA = "/Users/thomas/projects/of_v0.12.1_osx_release/examples/sound/soundPlayerFFTExample/bin/data/";

var beat    = new of.SoundPlayer();
var ow      = new of.SoundPlayer();
var dog     = new of.SoundPlayer();
var rooster = new of.SoundPlayer();

// ball state
var px = 300, py = 300;
var vx = 3.5, vy = 2.2;
var prevMouseX = 0, prevMouseY = 0;

// rolling velocity history (stands in for the FFT bars)
var VEL_BINS   = 128;
var velHistory = [];
for (var i = 0; i < VEL_BINS; i++) velHistory.push(0);

function setup() {
    of.setWindowShape(760, 600);
    of.setFrameRate(60);

    beat.load(DATA + "sounds/jdee_beat.mp3");
    ow.load(DATA + "sounds/ow.mp3");
    dog.load(DATA + "sounds/dog.mp3");
    rooster.load(DATA + "sounds/rooster.mp3");

    console.log("Sound bouncing ready — drag the mouse to fling the ball");
}

function update() {
    of.setWindowTitle("sound-bouncing  |  " + Math.round(of.getFrameRate()) + " fps");

    px += vx;
    py += vy;

    var vel = Math.sqrt(vx * vx + vy * vy);

    // left / right wall
    if (px < 0) {
        px = 0;  vx *= -1;
        dog.setVolume(Math.min(vel / 5.0, 1));
        dog.play();
    } else if (px > of.getWidth()) {
        px = of.getWidth();  vx *= -1;
        ow.setVolume(Math.min(vel / 5.0, 1));
        ow.play();
    }

    // top / bottom wall
    if (py < 0) {
        py = 0;  vy *= -1;
        rooster.setVolume(Math.min(vel / 5.0, 1));
        rooster.play();
    } else if (py > of.getHeight()) {
        py = of.getHeight();  vy *= -1;
        beat.setVolume(Math.min(vel / 5.0, 1));
        beat.play();
    }

    // friction
    vx *= 0.996;
    vy *= 0.996;

    // update velocity history
    vel = Math.sqrt(vx * vx + vy * vy);
    velHistory.push(vel);
    if (velHistory.length > VEL_BINS) velHistory.shift();
}

function draw() {
    of.background(80, 80, 20);

    var scrW   = of.getWidth();
    var scrH   = of.getHeight();
    var barAreaX = 100;
    var barAreaW = 5 * VEL_BINS;
    var barAreaH = 200;
    var barAreaY = scrH - 100;   // bottom of the bars

    // background rect for velocity history
    of.enableAlphaBlending();
    of.setColor(255, 255, 255, 100);
    of.fill();
    of.drawRectangle(barAreaX, barAreaY - barAreaH, barAreaW, barAreaH);
    of.disableAlphaBlending();

    // velocity history bars (replaces FFT — ofSoundGetSpectrum not available)
    of.setColor(255, 255, 255, 255);
    of.fill();
    var barW = 5;
    for (var i = 0; i < velHistory.length; i++) {
        var bh = of.map(velHistory[i], 0, 15, 0, barAreaH, true);
        of.drawRectangle(barAreaX + i * barW, barAreaY, barW - 1, -bh);
    }

    // ball glow
    of.enableAlphaBlending();
    of.setColor(255, 255, 255, 20);
    of.drawCircle(px, py, 50);
    of.disableAlphaBlending();

    // ball
    of.setColor(255, 255, 255, 255);
    of.fill();
    of.drawCircle(px, py, 8);
}

function mouseDragged(x, y, button) {
    vx += (x - prevMouseX) / 20.0;
    vy += (y - prevMouseY) / 20.0;
    prevMouseX = x;
    prevMouseY = y;
}

function mousePressed(x, y, button) {
    prevMouseX = x;
    prevMouseY = y;
}

setup();
