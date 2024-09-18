// import * as std from "std";

function setup() {
  console.log("js -> setup");
  // console.log(`vars set from oF: ${testInt}, ${testFloat}, ${testString}`);
  // of.setWindowTitle("example-gameObject");
  // of.setClipboardString("js -> setup");
  // of.setFrameRate(60);
  // of.background(32, 32, 32);
}

function update() {
  // console.log(`js -> update, ${of.getFrameNum()}`);
  // x = (x + 1) % of.getWidth();
  // y = (y + 1) % of.getHeight();
}

function draw() {
  // console.log(`js -> draw, ${of.getFrameNum()}`);
  // const frameNum = of.getFrameNum();
  // of.setColor(frameNum % 255, 100, 255);
  // // console.log(`test ${ofGetMouseX()}`);
  // of.drawRectangle(
  //   mousePressed ? ofGetMouseX() : frameNum % of.getWidth(),
  //   mousePressed ? ofGetMouseY() : frameNum % of.getHeight(),
  //   height,
  //   height
  // );
}
function exit() {
  console.log("js -> exit");
}

function keyPressed(key) {
  console.log(`js -> key pressed - ${key}`);
}

function keyReleased(key) {
  console.log(`js -> key released - ${key}`);
  // switch (key) {
  //   case "f": {
  //     of.toggleFullscreen();
  //     break;
  //   }
  //   // case "t": {
  //   //   // TODO: fix timeout, or implement it in js.h
  //   //   std.setTimeout(() => {
  //   //     console.log("timeout after 5s");
  //   //   }, 5000);
  //   //   break;
  //   // }
  // }
  // testInt++;
}

function mouseMoved(x, y) {
  console.log(`js -> mouse moved - (${x}, ${y})`);
}

function mouseDragged(x, y) {
  console.log(`js -> mouse dragged - (${x}, ${y})`);
}

function mousePressed(x, y, button) {
  console.log(`js -> mouse pressed - ((${x}, ${y}), ${button})`);
}

function mouseReleased(x, y, button) {
  console.log(`js -> mouse released - ((${x}, ${y}), ${button})`);
}

function mouseEntered(x, y) {
  console.log(`js -> mouse entered - (${x}, ${y})`);
}

function mouseExited(x, y) {
  console.log(`js -> mouse exited - (${x}, ${y})`);
}

function windowResized(width, height) {
  console.log(`js -> windowResized (${width}, ${height})`);
}

function dragEvent(x, y, files) {
  console.log(`js -> dragEvent (${x}, ${y})`);
  // files.forEach((file) => {
  //   console.log(file);
  // });
}
