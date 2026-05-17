// Plasma shader example
// Demonstrates: of.Shader, uniform updates, mouse interaction, hot-reload
//
// Run with:
//   ./ofJsRuntime -f /path/to/shader.js
//   ./ofJsRuntime -f /path/to/shader.js --watch   (live-reload on save)
//
// Tweak `speed` and `complexity` while --watch is running to see changes live.

var shader     = new of.Shader();
var speed      = 0.6;
var complexity = 8.0;
var mesh;

function setup() {
    // __dirname is the absolute directory of this script file,
    // injected by ofJsRuntime so paths work regardless of cwd.
    var shaderBase = __dirname + "/shaders/plasma";
    var ok = shader.load(shaderBase + ".vert", shaderBase + ".frag");
    if (!ok) {
        console.log("Shader failed to load from: " + shaderBase);
        return;
    }
    console.log("Shader loaded OK");

    // Full-screen quad mesh — reuse across frames
    mesh = new of.Mesh("triangle_strip");
    mesh.addTexCoord(0, 0); mesh.addVertex(0, 0, 0);
    mesh.addTexCoord(1, 0); mesh.addVertex(of.getWidth(), 0, 0);
    mesh.addTexCoord(0, 1); mesh.addVertex(0, of.getHeight(), 0);
    mesh.addTexCoord(1, 1); mesh.addVertex(of.getWidth(), of.getHeight(), 0);
}

function update() {
    of.setWindowTitle("plasma shader  |  " + Math.round(of.getFrameRate()) + " fps");
}

function draw() {
    of.background(0);

    if (!shader.isLoaded()) {
        of.setColor(200, 80, 80);
        return;
    }

    var w  = of.getWidth();
    var h  = of.getHeight();
    var mx = of.getMouseX() / w;
    var my = of.getMouseY() / h;

    shader.begin();
    shader.setUniform1f("time",       of.getElapsedTimef());
    shader.setUniform2f("resolution", w, h);
    shader.setUniform2f("mouse",      mx, my);
    shader.setUniform1f("speed",      speed);
    shader.setUniform1f("complexity", complexity);
    mesh.draw();
    shader.end();
}

// Click to toggle between calm and chaotic
function mousePressed(x, y, button) {
    if (complexity < 12) {
        complexity = 18;
        speed      = 1.4;
    } else {
        complexity = 8;
        speed      = 0.6;
    }
}

// Scroll wheel adjusts speed
function keyPressed(key) {
    if (key === 43 || key === 61) speed      = Math.min(speed + 0.1, 4.0);   // +/=
    if (key === 45)               speed      = Math.max(speed - 0.1, 0.05);  // -
    if (key === 91)               complexity = Math.max(complexity - 1, 1);  // [
    if (key === 93)               complexity = Math.min(complexity + 1, 30); // ]
}

