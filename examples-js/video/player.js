// Port of openFrameworks examples/video/videoPlayerExample
//
// Loads fingers.mp4 and displays it alongside a circle visualisation
// driven by pixel brightness sampled from the movie frame.
//
// Controls:
//   f         — toggle frame-by-frame / speed-control mode
//   ← / →     — previous / next frame  (frame-by-frame mode)
//   0         — jump to first frame
//   Mouse X   — set playback speed      (speed-control mode)
//   Click     — pause / unpause         (speed-control mode)
//   Drag X    — scrub position          (speed-control mode)
//
// The movie file is read from the OF example's data folder. Adjust the path
// below if you keep the file elsewhere.
//
// Run:
//   ./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime \
//     -f ../examples-js/video/player.js

var OF_KEY_LEFT  = 57356;
var OF_KEY_RIGHT = 57358;

var moviePath = "/Users/thomas/projects/of_v0.12.1_osx_release/examples/video/videoPlayerExample/bin/data/movies/fingers.mp4";

var movie        = new of.VideoPlayer();
var frameByframe = false;

function setup() {
    of.background(255, 255, 255);
    of.setFrameRate(60);

    var ok = movie.load(moviePath);
    if (!ok) {
        console.log("ERROR: could not load movie at: " + moviePath);
        return;
    }
    movie.setLoop(true);
    movie.play();
    console.log("Loaded movie: " + movie.getWidth() + "x" + movie.getHeight()
                + "  " + movie.getTotalNumFrames() + " frames");
}

function update() {
    movie.update();
    of.setWindowTitle("video-player  |  " + Math.round(of.getFrameRate()) + " fps");
}

function draw() {
    of.background(255, 255, 255);
    of.setColor(255, 255, 255, 255);
    movie.draw(20, 20);

    // Sample pixel brightness every 8 pixels and draw scaled circles
    if (movie.isLoaded() && movie.isFrameNew()) {
        var vidW = movie.getWidth();
        var vidH = movie.getHeight();
        of.setColor(0, 0, 0, 180);
        for (var i = 4; i < vidW; i += 8) {
            for (var j = 4; j < vidH; j += 8) {
                var c = movie.getPixelColor(i, j);
                var brightness = (c.r + c.g + c.b) / 3;
                var val = 1 - (brightness / 255);
                of.drawCircle(400 + i, 20 + j, 10 * val);
            }
        }
    }

    // HUD
    of.setColor(0, 0, 0, 255);
    of.drawBitmapString("press f to change", 20, 320);

    if (!frameByframe) {
        of.setColor(0, 0, 0, 255);
    } else {
        of.setColor(180, 180, 180, 255);
    }
    of.drawBitmapString("mouse speed position", 20, 340);

    if (frameByframe) {
        of.setColor(0, 0, 0, 255);
    } else {
        of.setColor(180, 180, 180, 255);
    }
    of.drawBitmapString("keys <- -> frame by frame", 190, 340);

    of.setColor(0, 0, 0, 255);
    of.drawBitmapString("frame: " + movie.getCurrentFrame() + " / " + movie.getTotalNumFrames(), 20, 380);
    var elapsed = (movie.getPosition() * movie.getDuration()).toFixed(2);
    var dur     = movie.getDuration().toFixed(2);
    of.drawBitmapString("duration: " + elapsed + " / " + dur, 20, 400);
    of.drawBitmapString("speed: " + movie.getSpeed().toFixed(2), 20, 420);

    if (movie.getIsMovieDone()) {
        of.setColor(255, 0, 0, 255);
        of.drawBitmapString("end of movie", 20, 440);
    }
}

function keyPressed(key) {
    if (key === 102) {  // 'f'
        frameByframe = !frameByframe;
        movie.pause(frameByframe);
    } else if (key === OF_KEY_LEFT) {
        movie.previousFrame();
    } else if (key === OF_KEY_RIGHT) {
        movie.nextFrame();
    } else if (key === 48) {  // '0'
        movie.firstFrame();
    }
}

function mouseMoved(x, y) {
    if (!frameByframe) {
        var pct   = x / of.getWidth();
        var speed = (2 * pct - 1) * 5.0;
        movie.setSpeed(speed);
    }
}

function mouseDragged(x, y, button) {
    if (!frameByframe) {
        var pct = x / of.getWidth();
        movie.setPosition(pct);
    }
}

function mousePressed(x, y, button) {
    if (!frameByframe) movie.pause(true);
}

function mouseReleased(x, y, button) {
    if (!frameByframe) movie.pause(false);
}

setup();
