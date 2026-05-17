let stored = [];
function update() {
    of.setWindowTitle(Math.round(of.getFrameRate()) + " fps");
}
function draw() {
    of.background(30, 30, 30);

    // draw stored circles
    of.setColor(120, 180, 255, 180);
    for (let c of stored) {
        of.drawCircle(c.x, c.y, 20);
    }

    // draw circle following the mouse
    of.setColor(255, 255, 255);
    of.drawCircle(of.getMouseX(), of.getMouseY(), 20);
}

function mousePressed(x, y, button) {
    stored.push({ x: x, y: y });
}
