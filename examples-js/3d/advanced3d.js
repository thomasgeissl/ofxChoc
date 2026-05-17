// Port of openFrameworks examples/3d/advanced3dExample
//
// A particle swarm driven by Simple Harmonic Motion (SHM).
//
// Physics (per particle, every frame):
//   1. position  += velocity * dt              (integrate velocity)
//   2. velocity  -= k * position * dt          (spring to origin)
//   3. velocity  -= k * (position - light) * dt (spring to light)
//   4. speed clamped to MAX_VELOCITY
//
// Result: particles orbit the origin with an emergent flocking-like quality.
// The white sphere (light) is particle[0]; the whole swarm is attracted to it.
//
// The original example adds ofLight / ofMaterial shading and shows four
// camera viewports (EasyCam, Front, Top, Left).  Here we use a single
// EasyCam and unlit coloured spheres — the motion is the same.
//
// Controls:
//   Mouse drag / scroll — orbit / zoom (EasyCam)
//   r                   — re-randomise swarm
//   f                   — fullscreen
//
// Run:
//   ./ofJsRuntime -f /path/to/3d/advanced3d.js

var SPRING_K     = 0.1;
var MAX_VELOCITY = 30.0;
var N_PARTICLES  = 100;

// ── Particle arrays ───────────────────────────────────────────────────────────

var positions  = [];
var velocities = [];
var colors     = [];

function initSwarm() {
    var posDist = 50;
    var velDist = 10;
    positions  = [];
    velocities = [];
    colors     = [];
    for (var i = 0; i < N_PARTICLES; i++) {
        positions.push(new of.Vec3(
            (Math.random() - 0.5) * posDist,
            (Math.random() - 0.5) * posDist,
            (Math.random() - 0.5) * posDist
        ));
        velocities.push(new of.Vec3(
            (Math.random() - 0.5) * velDist,
            (Math.random() - 0.5) * velDist,
            (Math.random() - 0.5) * velDist
        ));
        // Colours: random red/green, fixed blue tint — matches the original
        colors.push(new of.Color(
            Math.floor(Math.random() * 255),
            Math.floor(Math.random() * 255),
            150, 255
        ));
    }
}

// ── Grid (built once as a lines mesh) ────────────────────────────────────────

function buildGrid(cellSize, cells) {
    var m    = new of.Mesh("lines");
    var half = (cells / 2) * cellSize;
    for (var i = -cells / 2; i <= cells / 2; i++) {
        // lines running along X
        m.addVertex(-half, 0, i * cellSize);
        m.addVertex( half, 0, i * cellSize);
        // lines running along Z
        m.addVertex(i * cellSize, 0, -half);
        m.addVertex(i * cellSize, 0,  half);
    }
    return m;
}

// ── Scene setup ───────────────────────────────────────────────────────────────

initSwarm();

var cam      = new of.EasyCam();
var gridMesh = buildGrid(100, 10);
var lastTime = 0;

// ── Update ────────────────────────────────────────────────────────────────────

function update() {
    var now = of.getElapsedTimef();
    var dt  = Math.min(now - lastTime, 0.05);   // cap to avoid first-frame explosion
    lastTime = now;

    var light = positions[0];   // first particle acts as the light / attractor

    for (var i = 0; i < N_PARTICLES; i++) {
        // [1] integrate velocity → position
        positions[i] = positions[i].add(velocities[i].mul(dt));

        // [2] spring force toward origin
        velocities[i] = velocities[i].add(positions[i].mul(-SPRING_K * dt));

        // [3] spring force toward the light (simple flocking-like behaviour)
        var toLight = positions[i].sub(light);
        velocities[i] = velocities[i].add(toLight.mul(-SPRING_K * dt));

        // [4] clamp speed
        var speed = velocities[i].length();
        if (speed > MAX_VELOCITY) {
            velocities[i] = velocities[i].mul(MAX_VELOCITY / speed);
        }
    }
}

// ── Draw ──────────────────────────────────────────────────────────────────────

function draw() {
    of.background(70, 70, 70);

    cam.begin();
    of.enableDepthTest();

    // Grid
    of.setColor(255, 100, 100);
    gridMesh.draw();

    // Swarm particles
    for (var i = 1; i < N_PARTICLES; i++) {
        var p = positions[i];
        var c = colors[i];
        of.setColor(c.r, c.g, c.b);
        of.drawSphere(p.x, p.y, p.z, 1.0);
    }

    // Light: the first particle, drawn larger and white
    var lp = positions[0];
    of.setColor(255, 255, 255);
    of.drawSphere(lp.x, lp.y, lp.z, 2.5);

    of.disableDepthTest();
    cam.end();

    // HUD
    of.setColor(255);
    of.drawBitmapString(
        "FPS: " + Math.round(of.getFrameRate()) + "\n\n" +
        "Particle swarm — Simple Harmonic Motion\n" +
        "Each particle springs to origin and tracks the light (white sphere)\n" +
        "r: re-randomise  |  f: fullscreen",
        20, 20
    );
}

// ── Input ─────────────────────────────────────────────────────────────────────

function keyPressed(key) {
    if (key === 114 || key === 82) initSwarm();            // r / R
    if (key === 102 || key === 70) of.toggleFullscreen();  // f / F
}
