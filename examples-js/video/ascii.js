// Port of openFrameworks examples/video/asciiVideoExample
// Original by Ben Fry, adapted for openFrameworks.
//
// Renders the webcam feed as ASCII art.  Each character tile is chosen by
// mapping pixel brightness through a power curve to a character ramp.
//
// Performance notes:
//   - tileW/tileH control the trade-off between resolution and speed.
//     Larger tiles = fewer getPixelColor + drawBitmapString calls per frame.
//   - ofDrawBitmapString is used instead of ofTrueTypeFont — it's much cheaper
//     for single-character calls.
//   - Mouse X cross-fades the raw video underneath the ASCII layer.
//
// Controls:
//   Mouse X — fade between ASCII and raw video
//   s / S   — open video settings panel
//
// Run:
//   ./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime \
//     -f ../examples-js/video/ascii.js

var camW = 640;
var camH = 480;

// Tile size: 14×18 gives ~45×26 ≈ 1200 samples/frame — fast enough for 30fps.
// Lower values (7×9) look sharper but halve the frame rate.
var tileW = 14;
var tileH = 18;

// Character ramp: space (dark) → Q (bright)
var ASCII_CHARS = "  ..,,,'''``--_:;^^**\"\"=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q";

var grabber = new of.VideoGrabber();

function setup() {
    of.background(0, 0, 0);
    of.setFrameRate(30);
    of.enableAlphaBlending();

    grabber.setVerbose(true);
    grabber.setup(camW, camH);

    console.log("ASCII example ready  —  tileW=" + tileW + " tileH=" + tileH
                + "  cells=" + Math.floor(camW/tileW) + "×" + Math.floor(camH/tileH));
}

function update() {
    grabber.update();
    of.setWindowTitle("ascii-video  |  " + Math.round(of.getFrameRate()) + " fps");
}

function draw() {
    of.background(0, 0, 0);

    // Raw video fades in with mouse X
    var alpha = of.map(of.getMouseX(), 0, of.getWidth(), 0, 255, true);
    of.setColor(255, 255, 255, alpha);
    grabber.draw(0, 0);

    // ASCII layer
    of.setColor(255, 255, 255, 255);
    for (var i = 0; i < camW; i += tileW) {
        for (var j = tileH; j < camH; j += tileH) {
            var c   = grabber.getPixelColor(i, j);
            var lightness = (c.r + c.g + c.b) / 3;
            var t   = lightness / 255;
            var idx = Math.floor(Math.pow(t, 2.5) * ASCII_CHARS.length);
            idx = Math.max(0, Math.min(idx, ASCII_CHARS.length - 1));
            of.drawBitmapString(ASCII_CHARS[idx], i, j);
        }
    }
}

function keyPressed(key) {
    if (key === 115 || key === 83)  // 's' / 'S'
        grabber.videoSettings();
}

setup();
