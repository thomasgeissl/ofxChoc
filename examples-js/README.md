# examples-js

JavaScript examples for the `ofJsRuntime` app. Each `.js` file can be run directly without compiling.

Open this folder in VS Code for autocompletion — `of.d.ts` and `jsconfig.json` cover the built-in `of.*` API.

## Running an example

From the `ofJsRuntime` directory:

```bash
./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime -f ../examples-js/<example>.js
```

Add `--watch` to auto-reload the file whenever you save it:

```bash
./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime -f ../examples-js/<example>.js --watch
```

## Available callbacks

Define any of these functions in your JS file and they will be called automatically:

| Function | Called when |
|---|---|
| `setup()` | once after the script loads (and again on hot-reload) |
| `draw()` | every frame, for drawing |
| `update()` | every frame, for logic |
| `mousePressed(x, y, button)` | mouse button pressed |
| `mouseReleased(x, y, button)` | mouse button released |
| `mouseMoved(x, y)` | mouse moved |
| `mouseDragged(x, y, button)` | mouse dragged |
| `keyPressed(key)` | key pressed |
| `keyReleased(key)` | key released |
| `windowResized(w, h)` | window resized |

## Available bindings (`of.*`)

All bindings live on the global `of` object:

**Graphics**
- `of.background(r, g, b, a=255)`
- `of.setColor(r, g, b, a=255)` or `of.setColor(grey)`
- `of.clear(r, g, b, a=0)`
- `of.fill()` / `of.noFill()`
- `of.setLineWidth(w)`
- `of.drawCircle(x, y, r)`
- `of.drawEllipse(x, y, w, h)`
- `of.drawRectangle(x, y, w, h)`
- `of.drawLine(x1, y1, x2, y2)`
- `of.drawCurve(x0,y0, x1,y1, x2,y2, x3,y3)`
- `of.drawBox(x, y, z, size)` / `of.drawBox(size)`
- `of.drawSphere(x, y, z, r)` / `of.drawSphere(r)`
- `of.drawBitmapString(str, x, y)`
- `of.beginShape()` / `of.vertex(x, y, z=0)` / `of.endShape(close=false)` — draw polylines and filled shapes

**Transforms / GL**
- `of.pushMatrix()` / `of.popMatrix()`
- `of.pushStyle()` / `of.popStyle()`
- `of.translate(x, y, z=0)`
- `of.scale(x, y, z=1)`
- `of.rotateDeg(deg, x=0, y=0, z=1)`
- `of.rotateXDeg(d)` / `of.rotateYDeg(d)` / `of.rotateZDeg(d)`
- `of.enableAlphaBlending()` / `of.disableAlphaBlending()`
- `of.enableDepthTest()` / `of.disableDepthTest()`

**Objects (classes)**
- `new of.Fbo()` — `.allocate(w,h,fmt)`, `.begin()`, `.end()`, `.draw(x,y)`
- `new of.Image()` — `.load(path)`, `.loadFromBase64(base64)`, `.save(path)`, `.draw(x,y,w,h)`, `.isAllocated()`
- `new of.Shader()` — `.load(vert,frag)`, `.begin()`, `.end()`, `.setUniform1f(name,v)`, etc.
- `new of.Mesh()` — `.addVertex(x,y,z)`, `.addTexCoord(u,v)`, `.draw()`
- `new of.VideoPlayer()` — `.load(path)`, `.play()`, `.update()`, `.draw(x,y,w,h)`, `.setSpeed(s)`, `.getSpeed()`, `.getCurrentFrame()`, `.getTotalNumFrames()`, `.getIsMovieDone()`, `.previousFrame()`, `.nextFrame()`, `.firstFrame()`, `.setFrame(n)`, `.getPixelColor(x,y)`
- `new of.VideoGrabber()` — `.setup(w,h)`, `.setDeviceID(id)`, `.setDesiredFrameRate(fps)`, `.update()`, `.draw(x,y,w,h)`, `.getPixelColor(x,y)`, `.listDevices()`
- `new of.SoundPlayer()` — `.load(path, stream=false)`, `.play()`, `.stop()`, `.setVolume(v)`, `.setPan(p)`, `.setSpeed(s)`, `.setLoop(bool)`, `.setMultiPlay(bool)`, `.isLoaded()`, `.isPlaying()`, `.getPosition()`, `.setPosition(pct)`
- `new of.SoundStream()` — `.setupInput(sr, bufSize, numCh)`, `.setupOutput(sr, bufSize, numCh)`, `.start()`, `.stop()`, `.getInputSamples()`, `.getInputRMS()`, `.pushOutputSamples(arr)`, `.getLastOutputSamples()`, `.getLastOutputRMS()`, `.getBufferSize()`, `.getSampleRate()`, `.getNumInputChannels()`, `.getNumOutputChannels()`
- `new of.Font()` — `.load(path,size)`, `.drawString(str,x,y)`
- `new of.Camera()` / `new of.EasyCam()` — `.begin()`, `.end()`
- `new of.Serial()` — `.setup(device,baud)`, `.readLine()`

**Pure JS math helpers**
- `new of.Vec2(x,y)` / `new of.Vec3(x,y,z)` — `.add()`, `.sub()`, `.length()`, `.lerp()`
- `new of.Color(r,g,b,a)` — `.lerp()`
- `new of.Rect(x,y,w,h)` — `.contains(x,y)`, `.getCenter()`

**Input**
- `of.getMouseX()`, `of.getMouseY()`
- `of.getPreviousMouseX()`, `of.getPreviousMouseY()`
- `of.getKeyPressed(keyCode)`
- `of.getMousePressed(button)`

**Application**
- `of.getWidth()`, `of.getHeight()`
- `of.setFrameRate(fps)`
- `of.setWindowTitle(title)`
- `of.setWindowShape(w, h)`
- `of.toggleFullscreen()`

**Time / Utils**
- `of.getElapsedTimef()`, `of.getElapsedTimeMillis()`, `of.getElapsedTimeMicros()`
- `of.getFrameRate()`, `of.getFrameNum()`
- `of.getHours()`, `of.getMinutes()`, `of.getSeconds()`
- `of.map(v, i0, i1, o0, o1, clamp=false)` — linear remap
- `of.clamp(v, min, max)`, `of.lerp(a, b, t)`, `of.norm(v, min, max)`

**Logging**
- `console.log(...)`, `console.warn(...)`, `console.error(...)`

**Globals injected by the runtime**
- `__dirname` — absolute path of the directory containing the currently running script

## Hot-reload tips

When using `--watch`, top-level `let`/`const` declarations will throw a redeclaration error on reload. Use `var` instead, or use the `||` guard pattern to preserve state across reloads:

```javascript
var stored = stored || [];   // keeps existing value on reload
var radius = radius || 20;   // only initializes on first load
```

## Chocons (dynamic addon bindings)

Chocons are shared libraries (`.dylib` on macOS, `.so` on Linux) that extend the JS runtime with extra APIs — typically wrapping an oF addon such as ofxOsc. They are loaded at runtime; you do not need to recompile `ofJsRuntime` to use them.

### `ofx.*` vs `of.*`

- Built-in bindings live on **`of.*`** (registered when the runtime starts).
- Chocon bindings live on **`ofx.*`** (e.g. `ofx.osc.Sender`). Each chocon chooses its own namespace under `ofx`.

### Where chocons are loaded from

When you run `-f path/to/script.js`, the runtime searches in order:

1. **`<scriptDir>/chocons/`** — next to your script (highest priority)
2. **`data/chocons/`** — bundled with the app (via `ofToDataPath`)

Only `.dylib` / `.so` files are loaded. If the same library name appears in both folders, the script-local copy wins.

For the OSC examples in `examples-js/chocons/`, the built library belongs at:

```
examples-js/chocons/chocons/ofxOscBindings.dylib
```

(on Linux: `ofxOscBindings.so`)

### Building a chocon

```bash
cd addons/ofxChoc/chocons/ofxOsc
make
```

This produces `ofxOscBindings.dylib` in that directory. Copy it into your script's `chocons/` folder (see path above for the bundled examples).

To author a new chocon, see `addons/ofxChoc/chocons/` — each chocon is a thin Makefile plus a binding `.cpp` that exports `ofxChoc_registerAddon` and `ofxChoc_clearChocon`.

### If a chocon is missing or fails to load

The runtime keeps running — a missing chocon is not fatal.

| Situation | What happens |
|---|---|
| `chocons/` folder missing | Skipped silently |
| `.dylib` / `.so` present but won't load | Error logged (`addon load failed: …`), file skipped |
| Loads successfully | Notice logged: `loaded chocon: ofxOscBindings.dylib` |

If the chocon never loaded but your script uses `ofx.*`, evaluation fails with a JS error such as `ReferenceError: ofx is not defined`. Code at the top level of the script (outside `setup()`) fails as soon as the file is loaded.

### OSC chocon API (`ofx.osc`)

**Receiver**

```javascript
var receiver = new ofx.osc.Receiver();
receiver.setup(port);
receiver.hasWaitingMessages();
var m = receiver.getNextMessage();  // { address, args, remoteHost, remotePort } or null
```

**Sender**

```javascript
var sender = new ofx.osc.Sender();
sender.setup(host, port);
sender.send(address, ...args);            // numbers, strings, bools
sender.sendBlobFromFile(address, path);   // returns bytes sent, or 0
sender.sendBlob(address, base64);         // returns bytes sent, or 0
```

**Blob arguments on receive** — blob args arrive as objects:

```javascript
{ type: "blob", size: 1234, base64: "..." }
```

Use `ofx.osc.isBlob(arg)` to test. To load a received image:

```javascript
receivedImage.loadFromBase64(m.args[0].base64);
```

Large blobs are base64-encoded for JS interop. Keep images small (same caveat as the C++ OSC examples) or split into multiple messages.

### Running the OSC examples

1. Build the chocon (see above) and copy it to `examples-js/chocons/chocons/`.
2. Add a PNG at `examples-js/chocons/data/of-logo.png` (same asset used by the C++ `oscSenderExample`).
3. Run receiver and sender in two terminals:

```bash
# terminal 1 — from ofJsRuntime/
./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime -f ../examples-js/chocons/oscReceiver.js

# terminal 2
./bin/ofJsRuntime.app/Contents/MacOS/ofJsRuntime -f ../examples-js/chocons/oscSender.js
```

Press **I** in the sender to transmit the logo as an OSC blob on `/image`. Mouse movement and button events are sent continuously / on click.

## Examples

| File | Description |
|---|---|
| `follow-mouse.js` | Circle follows the mouse; click to store circles |
| `shader.js` | Full-screen plasma shader with mouse/keyboard controls |
| `fbo-trails.js` | Port of fboTrailsExample — two FBOs (8-bit & float) with motion trails |
| `video/player.js` | Port of videoPlayerExample — playback controls, speed, pixel-circle viz |
| `video/grabber.js` | Port of videoGrabberExample — live webcam + colour-inverted copy |
| `video/ascii.js` | Port of asciiVideoExample — webcam rendered as ASCII characters |
| `video/slitscan.js` | Port of slitscanRadialClockExample — slitscan clock with radial display |
| `sound/player.js` | Port of soundPlayerExample — three sound files; click to play, mouse Y/X for speed/pan, drag middle column for beat speed |
| `sound/bouncing.js` | Port of soundPlayerFFTExample — ball bounces off walls triggering sounds; drag mouse to fling; velocity history graph |
| `sound/audio-input.js` | Port of audioInputExample — mic capture; left/right waveforms + volume circle + history graph |
| `sound/audio-output.js` | Port of audioOutputExample — sine/noise synthesis; mouse Y=freq, X=pan; click for noise |
| `sound/sound-buffer.js` | Port of soundBufferExample — 3 detuned sines × 3 LFOs; waveform width tracks RMS |
| `3d/easy-cam.js` | Port of easyCamExample — six coloured shapes on the three world axes; drag/scroll to orbit |
| `3d/box-cloud.js` | Port of ofBoxExample — 100 boxes drifting on noise-driven trajectories with coloured wireframe halos |
| `3d/ribbon.js` | Port of cameraRibbonExample — mouse trail becomes a tapered 3D ribbon; Space to orbit the sculpture |
| `3d/primitives.js` | Port of 3DPrimitivesExample — sphere, box, and mesh-built torus/cylinder/cone/plane; auto-spin + wireframe toggle |
| `3d/advanced3d.js` | Port of advanced3dExample — 100-particle swarm with Simple Harmonic Motion; springs to origin + light attractor |
| `chocons/oscSender.js` | Port of oscSenderExample — sends mouse position, button events, and image blobs to localhost:12345 (requires chocon; see **Chocons** above) |
| `chocons/oscReceiver.js` | Port of oscReceiveExample — listens on port 12345, displays remote mouse and received image blobs |
