// Port of openFrameworks examples/3d/cameraRibbonExample
// Original by James George (OFDevCon 2012)
//
// Move the mouse to paint a 3D ribbon trail.
// The ribbon fans out perpendicular to the path direction and tapers at its tail.
// All points descend in Z each frame, sculpting the trail into depth.
//
// Controls:
//   Mouse move    — add ribbon points
//   Space         — toggle orbit camera (mouse X = rotation angle)
//   Up arrow      — increase smoothing
//   Down arrow    — decrease smoothing
//
// Key codes: Space=32, Up=0xe00D (57357), Down=0xe00F (57359)
//
// Run:
//   ./ofJsRuntime -f /path/to/3d/ribbon.js

var points      = [];   // raw mouse trail (Vec3 array)
var useCamera   = false;
var smoothing   = 10;   // moving-average half-radius

var cam  = new of.Camera();
var mesh = new of.Mesh("triangle_strip");

// ── Helpers ───────────────────────────────────────────────────────────────────

// Rotate Vec3 v by angleDeg around unit axis (ax, ay, az) — Rodrigues' formula.
function rotateAround(v, angleDeg, ax, ay, az) {
    var rad = angleDeg * Math.PI / 180;
    var c   = Math.cos(rad);
    var s   = Math.sin(rad);
    var len = Math.sqrt(ax*ax + ay*ay + az*az);
    if (len > 0.00001) { ax /= len; ay /= len; az /= len; }
    var dot = v.x*ax + v.y*ay + v.z*az;
    return new of.Vec3(
        v.x*c + (ay*v.z - az*v.y)*s + ax*dot*(1 - c),
        v.y*c + (az*v.x - ax*v.z)*s + ay*dot*(1 - c),
        v.z*c + (ax*v.y - ay*v.x)*s + az*dot*(1 - c)
    );
}

// Moving-average smoother over a Vec3 array.
function smoothPoints(pts, r) {
    if (pts.length < 3 || r < 1) return pts;
    var out = [];
    for (var i = 0; i < pts.length; i++) {
        var sx = 0, sy = 0, sz = 0, n = 0;
        var lo = Math.max(0, i - r);
        var hi = Math.min(pts.length - 1, i + r);
        for (var j = lo; j <= hi; j++) {
            sx += pts[j].x; sy += pts[j].y; sz += pts[j].z; n++;
        }
        out.push(new of.Vec3(sx / n, sy / n, sz / n));
    }
    return out;
}

// ── Update ────────────────────────────────────────────────────────────────────

function update() {
    // Trim oldest points
    if (points.length > 250) points.splice(0, points.length - 250);

    // Descend all points in Z to sculpt the ribbon into depth
    if (!useCamera) {
        for (var i = 0; i < points.length; i++) {
            points[i] = new of.Vec3(points[i].x, points[i].y, points[i].z - 4);
        }
    }

    // Rebuild the triangle-strip ribbon mesh from (optionally smoothed) points
    mesh.clear();
    mesh.setMode("triangle_strip");

    var verts = smoothPoints(points, smoothing);

    for (var i = 1; i < verts.length; i++) {
        var thisPoint = verts[i - 1];
        var nextPoint = verts[Math.min(i, verts.length - 1)];

        var dir  = nextPoint.sub(thisPoint);
        var dist = dir.length();
        if (dist < 0.001) continue;

        var unit    = dir.normalize();
        var toLeft  = rotateAround(unit, -90, 0, 0, 1);
        var toRight = rotateAround(unit,  90, 0, 0, 1);

        // Ribbon narrows when points are close together (fast motion)
        var thickness = of.map(dist, 0, 60, 20, 2, true);
        // Taper the leading tail to zero
        if (i < 25) thickness *= of.map(i, 0, 25, 0.0, 1.0, true);

        var lp = thisPoint.add(toLeft.mul(thickness));
        var rp = thisPoint.add(toRight.mul(thickness));

        mesh.addVertex(lp.x, lp.y, lp.z);
        mesh.addVertex(rp.x, rp.y, rp.z);
    }
}

// ── Draw ──────────────────────────────────────────────────────────────────────

function draw() {
    of.background(255);
    of.setColor(0);

    if (useCamera && points.length > 1) {
        // Orbit the whole sculpture: compute centroid, position camera on a circle
        var cx = 0, cy = 0, cz = 0;
        for (var i = 0; i < points.length; i++) {
            cx += points[i].x; cy += points[i].y; cz += points[i].z;
        }
        cx /= points.length; cy /= points.length; cz /= points.length;

        var first = points[0];
        var dx = first.x - cx, dy = first.y - cy, dz = first.z - cz;
        var orbitR = Math.sqrt(dx*dx + dy*dy + dz*dz) + 200;
        var angle  = of.map(of.getMouseX(), 0, of.getWidth(), 0, Math.PI * 2);

        cam.setPosition(cx + Math.sin(angle) * orbitR, cy, cz + Math.cos(angle) * orbitR);
        cam.lookAt(cx, cy, cz);

        cam.begin();
        mesh.draw();
        cam.end();
    } else {
        // Flat 2D view — just draw the ribbon directly into screen space
        mesh.draw();
    }

    // HUD
    of.setColor(40);
    of.drawBitmapString(
        "Move mouse to paint  |  Space: orbit (" + (useCamera ? "ON" : "off") + ")" +
        "  |  Up/Down: smooth (" + smoothing + ")",
        20, of.getHeight() - 20
    );
}

// ── Input ─────────────────────────────────────────────────────────────────────

function mouseMoved(x, y) {
    if (!useCamera) {
        points.push(new of.Vec3(x, y, 0));
    }
}

function keyPressed(key) {
    if (key === 32)    useCamera = !useCamera;                         // Space
    if (key === 57357) smoothing = Math.min(smoothing + 1, 40);       // OF_KEY_UP
    if (key === 57359) smoothing = Math.max(smoothing - 1, 0);        // OF_KEY_DOWN
}
