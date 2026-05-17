# examples-js

JavaScript examples for the `ofJsRuntime` app. Each `.js` file can be run directly without compiling.

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
- `new of.Image()` — `.load(path)`, `.draw(x,y,w,h)`, `.saveImage(path)`
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
