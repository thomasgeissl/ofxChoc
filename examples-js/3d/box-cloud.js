// Port of openFrameworks examples/3d/ofBoxExample
//
// 100 boxes drifting through 3D space on smooth noise-driven trajectories.
// Each box has a solid white fill and a coloured wireframe halo.
//
// Run:
//   ./ofJsRuntime -f /path/to/3d/box-cloud.js
//   ./ofJsRuntime -f /path/to/3d/box-cloud.js --watch

var cam = new of.EasyCam();

// ── Value noise ───────────────────────────────────────────────────────────────
// Smooth 1-D value noise in [0, 1].
function vnoise(x) {
    var i  = Math.floor(x);
    var f  = x - i;
    f = f * f * (3.0 - 2.0 * f);                          // smoothstep
    var h0 = Math.abs(Math.sin(i       * 127.1 + 311.7));
    var h1 = Math.abs(Math.sin((i + 1) * 127.1 + 311.7));
    return (h0 + (h1 - h0) * f) % 1.0;
}

// Signed noise in [-1, 1].  Different (x,y,z) weights give independent axes.
function snoise(x, y, z) {
    return vnoise(x * 1.7 + y * 2.3 + z * 3.1) * 2.0 - 1.0;
}

// ── Draw ──────────────────────────────────────────────────────────────────────
function draw() {
    of.background(0);

    var cloudSize     = of.getWidth() * 0.5;
    var movementSpeed = 0.1;
    var maxBoxSize    = 100;
    var boxCount      = 100;

    cam.begin();
    of.enableDepthTest();

    for (var i = 0; i < boxCount; i++) {
        var t  = (of.getElapsedTimef() + i) * movementSpeed;

        // Three independent smooth trajectories — one per axis
        var px = snoise(t, 0, 0) * cloudSize;
        var py = snoise(0, t, 0) * cloudSize;
        var pz = snoise(0, 0, t) * cloudSize;

        var boxSize = maxBoxSize * vnoise(px * 0.01 + py * 0.007 + pz * 0.005 + t * 0.3);

        of.pushMatrix();
        of.translate(px, py, pz);
        of.rotateXDeg(px);
        of.rotateYDeg(py);
        of.rotateZDeg(pz);

        // Solid white fill
        of.fill();
        of.setColor(255, 255, 255);
        of.drawBox(0, 0, 0, boxSize);

        // Coloured wireframe halo — hue cycles per box and over time
        var hue = ((Math.sin(t) * 0.5 + 0.5) * 360 + i * 3.6) % 360;
        var c   = of.Color.fromHSB(hue, 1.0, 1.0);
        of.noFill();
        of.setColor(c.r, c.g, c.b);
        of.drawBox(0, 0, 0, boxSize * 1.1);

        of.popMatrix();
    }

    of.disableDepthTest();
    cam.end();

    of.setColor(180);
    of.drawBitmapString("box-cloud  |  " + Math.round(of.getFrameRate()) + " fps", 10, 20);
}
