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
  if(of.getMousePressed(0)){
    // console.log("mouse pressed");
  }
  if(of.getKeyPressed("c".charCodeAt(0))){
    // console.log("c pressed");
  }
}

function draw() {
  // console.log(`js -> draw, ${of.getFrameNum()}`);
  const frameNum = of.getFrameNum();
  of.setColor(frameNum % 255, 100, 255);
  of.drawCircle(of.getFrameNum()%of.getWidth(), of.getFrameNum()%of.getHeight(), 30);

  const mouseX = of.getMouseX();
  const mouseY = of.getMouseY();
  of.setColor(0,0,0);
  of.drawCircle(mouseX, mouseY, mouseY/of.getHeight() * 20);


  if(of.getKeyPressed('c'.charCodeAt(0))){
    console.log("draw curve")
    of.drawCurve(0,0, 100,100,400,400, of.getWidth(), of.getHeight());
  }
}
function exit() {
  console.log("js -> exit");
}

function keyPressed(key) {
  console.log(`js -> key pressed - ${key}`);

  switch(key){
    case 'b'.charCodeAt(0): {
      of.background(Math.floor(Math.random()*255), 0, 0);
      break;
    }
    case 'f'.charCodeAt(0): {
      of.toggleFullscreen();
      break;
    }
    case 'u'.charCodeAt(0): {
      console.log("testing utils")
      console.log(`frameNum: ${of.getFrameNum()}`);
      // console.log(`getEnv(USER)${of.getEnv("USER")}`);
      console.log(`elapsedTimef: ${of.getElapsedTimef()}`);
      console.log(`elapsedTimeMicros: ${of.getElapsedTimeMicros()}`);
      console.log(`elapsedTimeMillis: ${of.getElapsedTimeMillis()}`);
      console.log(`hours: ${of.getHours()}`);
      console.log(`minutes: ${of.getMinutes()}`);
      console.log(`seconds: ${of.getSeconds()}`);
      console.log(`month: ${of.getMonth()}`);
      // console.log(`versionInfo: ${of.getVersionInfo()}`);
      // console.log(`versionMajor: ${of.getVersionMajor()}`);
      // console.log(`versionMinor: ${of.getVersionMinor()}`);
      // console.log(`versionPatch: ${of.getVersionPatch()}`);
      // console.log("systemTimeMicros", of.getSystemTimeMicros());
      // console.log("systemTimeMillis", of.getSystemTimeMillis());
      // console.log("unixTime", of.getUnixTime());
      // console.log("weekday", of.getWeekday());
      break;
    }
    // case "t": {
    //   // TODO: fix timeout, or implement it in js.h
    //   std.setTimeout(() => {
    //     console.log("timeout after 5s");
    //   }, 5000);
    //   break;
    // }
  }
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
