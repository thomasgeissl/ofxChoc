// Port of openFrameworks examples/3d/3DPrimitivesExample  (simplified)
//
// Six 3D primitives in a 2 × 3 grid, auto-spinning with EasyCam:
//
//   sphere   |   box   |   torus
//   cylinder |   cone  |   plane
//
// Sphere and box use built-in draw calls.
// Torus, cylinder, cone, and plane are built from of.Mesh so you can
// see how custom indexed / strip geometry works.
//
// Controls:
//   Mouse drag / scroll — EasyCam orbit / zoom
//   w                   — toggle wireframe
//   f                   — fullscreen
//
// Run:
//   ./ofJsRuntime -f /path/to/3d/primitives.js

// ── Mesh builders ─────────────────────────────────────────────────────────────

// Cylinder body as a triangle-strip around two parallel rings.
function buildCylinder(r, h, segs) {
    var m = new of.Mesh("triangle_strip");
    for (var i = 0; i <= segs; i++) {
        var a = (i / segs) * Math.PI * 2;
        m.addVertex(Math.cos(a) * r, -h * 0.5, Math.sin(a) * r);
        m.addVertex(Math.cos(a) * r,  h * 0.5, Math.sin(a) * r);
    }
    return m;
}

// Circular end-cap as a triangle-fan.
function buildCap(r, y, segs) {
    var m = new of.Mesh("triangle_fan");
    m.addVertex(0, y, 0);
    for (var i = 0; i <= segs; i++) {
        var a = (i / segs) * Math.PI * 2;
        m.addVertex(Math.cos(a) * r, y, Math.sin(a) * r);
    }
    return m;
}

// Cone body as a triangle-fan from tip to base ring.
function buildCone(r, h, segs) {
    var m = new of.Mesh("triangle_fan");
    m.addVertex(0, h * 0.5, 0);
    for (var i = 0; i <= segs; i++) {
        var a = (i / segs) * Math.PI * 2;
        m.addVertex(Math.cos(a) * r, -h * 0.5, Math.sin(a) * r);
    }
    return m;
}

// Torus as indexed triangles.  R = major radius, r = tube radius.
function buildTorus(R, r, rings, segs) {
    var m = new of.Mesh("triangles");
    for (var i = 0; i < rings; i++) {
        var phi = (i / rings) * Math.PI * 2;
        for (var j = 0; j < segs; j++) {
            var theta = (j / segs) * Math.PI * 2;
            m.addVertex(
                (R + r * Math.cos(theta)) * Math.cos(phi),
                r * Math.sin(theta),
                (R + r * Math.cos(theta)) * Math.sin(phi)
            );
        }
    }
    for (var i = 0; i < rings; i++) {
        for (var j = 0; j < segs; j++) {
            var a =  i              * segs + j;
            var b =  i              * segs + (j + 1) % segs;
            var c = ((i + 1) % rings) * segs + j;
            var d = ((i + 1) % rings) * segs + (j + 1) % segs;
            m.addIndex(a); m.addIndex(b); m.addIndex(d);
            m.addIndex(a); m.addIndex(d); m.addIndex(c);
        }
    }
    return m;
}

// Flat plane as a subdivided quad grid, lying in the XZ plane.
function buildPlane(w, d, cols, rows) {
    var m = new of.Mesh("triangles");
    for (var row = 0; row <= rows; row++) {
        for (var col = 0; col <= cols; col++) {
            m.addVertex((col / cols - 0.5) * w, 0, (row / rows - 0.5) * d);
        }
    }
    for (var row = 0; row < rows; row++) {
        for (var col = 0; col < cols; col++) {
            var a =  row      * (cols + 1) + col;
            var b =  row      * (cols + 1) + col + 1;
            var c = (row + 1) * (cols + 1) + col;
            var d = (row + 1) * (cols + 1) + col + 1;
            m.addIndex(a); m.addIndex(b); m.addIndex(d);
            m.addIndex(a); m.addIndex(d); m.addIndex(c);
        }
    }
    return m;
}

// ── Geometry (built once) ─────────────────────────────────────────────────────

var SEGS = 24;

var cylinderBody = buildCylinder(50, 120, SEGS);
var cylinderCapT = buildCap(50,  60, SEGS);
var cylinderCapB = buildCap(50, -60, SEGS);
var coneBody     = buildCone(60, 130, SEGS);
var coneBase     = buildCap(60, -65, SEGS);
var torus        = buildTorus(60, 22, SEGS, 16);
var plane        = buildPlane(160, 160, 6, 6);

// ── State ─────────────────────────────────────────────────────────────────────

var cam       = new of.EasyCam();
var wireframe = false;

// Per-primitive colours (one per cell)
var COLORS = [
    [255,  80,  80],   // sphere    — red
    [ 80, 200,  80],   // box       — green
    [ 80, 140, 255],   // torus     — blue
    [255, 220,  60],   // cylinder  — yellow
    [220,  80, 220],   // cone      — magenta
    [ 60, 220, 200],   // plane     — cyan
];

// ── Helpers ───────────────────────────────────────────────────────────────────

function drawPrimitive(solidFn) {
    if (wireframe) {
        of.noFill();
        solidFn();
    } else {
        of.fill();
        solidFn();
    }
}

// Translate to (cx, cy, 0), apply two-axis spin, run drawFn, restore.
function drawCell(cx, cy, sx, sy, colorIdx, drawFn) {
    var c = COLORS[colorIdx];
    of.setColor(c[0], c[1], c[2]);
    of.pushMatrix();
    of.translate(cx, cy, 0);
    of.rotateXDeg(sx);
    of.rotateYDeg(sy);
    drawFn();
    of.popMatrix();
}

// ── Draw ──────────────────────────────────────────────────────────────────────

function draw() {
    of.background(20);

    var t  = of.getElapsedTimef();
    var sx = Math.sin(t * 0.35) * 360;   // slow X wobble
    var sy = Math.cos(t * 0.075) * 360;  // even slower Y spin

    var col = 220;
    var row = 200;

    cam.begin();
    of.enableDepthTest();

    // Row 1
    drawCell(-col, -row * 0.5, sx, sy, 0, function() {
        drawPrimitive(function() { of.drawSphere(0, 0, 0, 80); });
    });
    drawCell(0, -row * 0.5, sx, sy, 1, function() {
        drawPrimitive(function() { of.drawBox(0, 0, 0, 130); });
    });
    drawCell(col, -row * 0.5, sx, sy, 2, function() {
        drawPrimitive(function() { torus.draw(); });
    });

    // Row 2
    drawCell(-col, row * 0.5, sx, sy, 3, function() {
        drawPrimitive(function() {
            cylinderBody.draw();
            cylinderCapT.draw();
            cylinderCapB.draw();
        });
    });
    drawCell(0, row * 0.5, sx, sy, 4, function() {
        drawPrimitive(function() {
            coneBody.draw();
            coneBase.draw();
        });
    });
    drawCell(col, row * 0.5, sx, sy, 5, function() {
        drawPrimitive(function() { plane.draw(); });
    });

    of.disableDepthTest();
    cam.end();

    // ── HUD ───────────────────────────────────────────────────────────────────
    of.setColor(200);
    var legend =
        "  sphere     box      torus\n" +
        " cylinder   cone     plane";
    of.drawBitmapString(legend, of.getWidth() * 0.5 - 130, of.getHeight() - 35);

    of.setColor(180);
    of.drawBitmapString(
        "FPS: " + Math.round(of.getFrameRate()) +
        "  |  w: wireframe (" + (wireframe ? "ON" : "off") + ")" +
        "  |  f: fullscreen",
        10, 20
    );
}

function keyPressed(key) {
    if (key === 119 || key === 87) wireframe = !wireframe;  // w / W
    if (key === 102 || key === 70) of.toggleFullscreen();   // f / F
}
