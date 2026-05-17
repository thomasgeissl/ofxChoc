// Type declarations for the ofxChoc JS runtime global `of` object.
// Drop this file (and jsconfig.json) into your examples-js folder to get
// IntelliSense / autocomplete for all `of.*` bindings in Cursor / VS Code.

// ── Return-value shapes ───────────────────────────────────────────────────────

interface OfPixelColor { r: number; g: number; b: number; }
interface OfPixelColorAlpha { r: number; g: number; b: number; a: number; }
interface OfPosition3 { x: number; y: number; z: number; }
interface OfVideoDevice { id: number; name: string; }
interface OfSerialDevice { id: number; path: string; name: string; }

// ── Class instance interfaces ─────────────────────────────────────────────────

interface OfFbo {
  /** Allocate the FBO. fmt defaults to GL_RGBA (6408). */
  allocate(w: number, h: number, fmt?: number, samples?: number): this;
  begin(): this;
  end(): this;
  draw(x: number, y: number, w?: number, h?: number): this;
  drawSubsection(x: number, y: number, w: number, h: number, sx: number, sy: number): this;
  getWidth(): number;
  getHeight(): number;
  isAllocated(): boolean;
  destroy(): void;
}

interface OfImage {
  load(path: string): boolean;
  save(path: string): this;
  allocate(w: number, h: number): this;
  draw(x: number, y: number, w?: number, h?: number): this;
  resize(w: number, h: number): this;
  grabScreen(x: number, y: number, w: number, h: number): this;
  update(): this;
  getWidth(): number;
  getHeight(): number;
  getColor(x: number, y: number): OfPixelColorAlpha;
  setColor(x: number, y: number, r: number, g: number, b: number, a?: number): this;
  isAllocated(): boolean;
  mirror(vertical: boolean, horizontal: boolean): this;
  destroy(): void;
}

interface OfShader {
  /** load(basePath) or load(vertPath, fragPath, geomPath?) */
  load(a: string, b?: string, geom?: string): boolean;
  begin(): this;
  end(): this;
  isLoaded(): boolean;
  setUniform1f(name: string, v: number): this;
  setUniform1i(name: string, v: number): this;
  setUniform2f(name: string, x: number, y: number): this;
  setUniform3f(name: string, x: number, y: number, z: number): this;
  setUniform4f(name: string, x: number, y: number, z: number, w: number): this;
  destroy(): void;
}

type OfMeshMode = 'triangles' | 'triangle_strip' | 'triangle_fan' | 'lines' | 'line_strip' | 'line_loop' | 'points';

interface OfMesh {
  addVertex(x: number, y: number, z?: number): this;
  /** Colors are 0–1 floats. */
  addColor(r: number, g: number, b: number, a?: number): this;
  addNormal(x: number, y: number, z?: number): this;
  addTexCoord(u: number, v: number): this;
  addIndex(i: number): this;
  draw(): this;
  drawWireframe(): this;
  drawVertices(): this;
  clear(): this;
  setMode(mode: OfMeshMode): this;
  getNumVertices(): number;
  getNumIndices(): number;
  enableColors(): this;
  disableColors(): this;
  enableNormals(): this;
  disableNormals(): this;
  destroy(): void;
}

interface OfVideoPlayer {
  load(path: string): boolean;
  play(): this;
  stop(): this;
  pause(p?: boolean): this;
  update(): this;
  draw(x: number, y: number, w?: number, h?: number): this;
  isFrameNew(): boolean;
  isPlaying(): boolean;
  isLoaded(): boolean;
  getIsMovieDone(): boolean;
  getWidth(): number;
  getHeight(): number;
  getDuration(): number;
  getPosition(): number;
  setPosition(pct: number): this;
  setVolume(v: number): this;
  setLoop(l?: boolean): this;
  setSpeed(s: number): this;
  getSpeed(): number;
  getCurrentFrame(): number;
  getTotalNumFrames(): number;
  previousFrame(): this;
  nextFrame(): this;
  firstFrame(): this;
  setFrame(n: number): this;
  getPixelColor(x: number, y: number): OfPixelColor;
  close(): this;
  destroy(): void;
}

interface OfVideoGrabber {
  setup(w?: number, h?: number): boolean;
  update(): this;
  draw(x: number, y: number, w?: number, h?: number): this;
  isFrameNew(): boolean;
  getWidth(): number;
  getHeight(): number;
  setDeviceID(id: number): this;
  setDesiredFrameRate(fps: number): this;
  videoSettings(): this;
  setVerbose(v?: boolean): this;
  getPixelColor(x: number, y: number): OfPixelColor;
  listDevices(): OfVideoDevice[];
  close(): this;
  destroy(): void;
}

interface OfSoundPlayer {
  load(path: string, stream?: boolean): boolean;
  unload(): this;
  play(): this;
  stop(): this;
  setVolume(v: number): this;
  setPan(p: number): this;
  setSpeed(s: number): this;
  setLoop(l?: boolean): this;
  setMultiPlay(m?: boolean): this;
  isLoaded(): boolean;
  isPlaying(): boolean;
  getPosition(): number;
  setPosition(pct: number): this;
  destroy(): void;
}

interface OfSoundStream {
  setupInput(sampleRate: number, bufferSize: number, numChannels: number): boolean;
  setupOutput(sampleRate: number, bufferSize: number, numChannels: number): boolean;
  start(): this;
  stop(): this;
  getSampleRate(): number;
  getBufferSize(): number;
  getNumInputChannels(): number;
  getNumOutputChannels(): number;
  /** Returns a Float64 array of the latest input buffer (interleaved channels). */
  getInputSamples(): number[];
  getInputRMS(): number;
  /** Push an array of floats into the output ring buffer. */
  pushOutputSamples(arr: number[]): this;
  getLastOutputSamples(): number[];
  getLastOutputRMS(): number;
  destroy(): void;
}

interface OfFont {
  load(path: string, size?: number, antiAlias?: boolean, fullCharSet?: boolean): boolean;
  drawString(str: string, x: number, y: number): this;
  drawStringAsShapes(str: string, x: number, y: number): this;
  getStringWidth(str: string): number;
  getStringHeight(str: string): number;
  getLineHeight(): number;
  isLoaded(): boolean;
  destroy(): void;
}

interface OfCameraBase {
  begin(): this;
  end(): this;
  setPosition(x: number, y: number, z: number): this;
  getPosition(): OfPosition3;
  lookAt(x: number, y: number, z: number): this;
  setFov(f: number): this;
  getFov(): number;
  setNearClip(n: number): this;
  setFarClip(f: number): this;
  tilt(deg: number): this;
  pan(deg: number): this;
  roll(deg: number): this;
  reset(): this;
  destroy(): void;
}

interface OfCamera extends OfCameraBase {}

interface OfEasyCam extends OfCameraBase {
  enableMouseInput(): this;
  disableMouseInput(): this;
  setDistance(d: number): this;
  getDistance(): number;
}

interface OfSerial {
  setup(portOrName: string | number, baud?: number): boolean;
  close(): this;
  isInitialized(): boolean;
  available(): number;
  readByte(): number;
  writeByte(b: number): this;
  readString(max?: number): string;
  writeString(s: string): this;
  flush(): this;
  listDevices(): OfSerialDevice[];
  destroy(): void;
}

interface OfVec2 {
  x: number; y: number;
  add(v: OfVec2): OfVec2;
  sub(v: OfVec2): OfVec2;
  mul(s: number): OfVec2;
  div(s: number): OfVec2;
  dot(v: OfVec2): number;
  length(): number;
  lengthSquared(): number;
  normalize(): OfVec2;
  distance(v: OfVec2): number;
  lerp(v: OfVec2, t: number): OfVec2;
  rotate(deg: number): OfVec2;
  angle(): number;
  set(x: number, y: number): this;
  clone(): OfVec2;
  toArray(): [number, number];
  toString(): string;
}

interface OfVec3 {
  x: number; y: number; z: number;
  add(v: OfVec3): OfVec3;
  sub(v: OfVec3): OfVec3;
  mul(s: number): OfVec3;
  div(s: number): OfVec3;
  dot(v: OfVec3): number;
  cross(v: OfVec3): OfVec3;
  length(): number;
  lengthSquared(): number;
  normalize(): OfVec3;
  distance(v: OfVec3): number;
  lerp(v: OfVec3, t: number): OfVec3;
  set(x: number, y: number, z: number): this;
  clone(): OfVec3;
  xy(): OfVec2;
  toArray(): [number, number, number];
  toString(): string;
}

interface OfColor {
  r: number; g: number; b: number; a: number;
  lerp(c: OfColor, t: number): OfColor;
  clamp(): OfColor;
  toHex(): number;
  toHexString(): string;
  withAlpha(a: number): OfColor;
  clone(): OfColor;
  toString(): string;
}

interface OfRect {
  x: number; y: number; width: number; height: number;
  readonly right: number;
  readonly bottom: number;
  readonly center: OfVec2;
  contains(px: number, py: number): boolean;
  containsPoint(v: OfVec2): boolean;
  intersects(r: OfRect): boolean;
  expand(dx: number, dy?: number): OfRect;
  scale(s: number): OfRect;
  translate(dx: number, dy: number): OfRect;
  clone(): OfRect;
  toString(): string;
}

// ── Static constructor shapes (for `new of.Fbo()` etc.) ───────────────────────

interface OfVec2Constructor {
  new(x?: number, y?: number): OfVec2;
  fromAngle(deg: number, len?: number): OfVec2;
}

interface OfVec3Constructor {
  new(x?: number, y?: number, z?: number): OfVec3;
}

interface OfColorConstructor {
  new(r?: number, g?: number, b?: number, a?: number): OfColor;
  fromHex(hex: number): OfColor;
  fromHexString(s: string): OfColor;
  fromHSB(h: number, s: number, b: number, a?: number): OfColor;
}

interface OfRectConstructor {
  new(x?: number, y?: number, w?: number, h?: number): OfRect;
  fromCenter(cx: number, cy: number, w: number, h: number): OfRect;
}

// ── The global `of` object ────────────────────────────────────────────────────

interface OfNamespace {
  // --- Graphics ---
  background(r: number, g: number, b: number, a?: number): void;
  /** Single grey value. */
  background(grey: number): void;
  setColor(r: number, g: number, b: number, a?: number): void;
  setColor(grey: number): void;
  clear(r: number, g: number, b: number, a?: number): void;
  clear(brightness: number, a?: number): void;
  fill(): void;
  noFill(): void;
  setLineWidth(w: number): void;
  drawCircle(x: number, y: number, r: number): void;
  drawEllipse(x: number, y: number, w: number, h: number): void;
  drawRectangle(x: number, y: number, w: number, h: number): void;
  drawLine(x1: number, y1: number, x2: number, y2: number): void;
  drawCurve(x0: number, y0: number, x1: number, y1: number, x2: number, y2: number, x3: number, y3: number): void;
  drawBox(size: number): void;
  drawBox(x: number, y: number, z: number, size: number): void;
  drawSphere(r: number): void;
  drawSphere(x: number, y: number, z: number, r: number): void;
  drawBitmapString(str: string, x: number, y: number): void;
  beginShape(): void;
  vertex(x: number, y: number, z?: number): void;
  endShape(close?: boolean): void;

  // --- Transforms / GL ---
  pushMatrix(): void;
  popMatrix(): void;
  pushStyle(): void;
  popStyle(): void;
  loadIdentityMatrix(): void;
  translate(x: number, y: number, z?: number): void;
  scale(x: number, y?: number, z?: number): void;
  rotateDeg(deg: number, x?: number, y?: number, z?: number): void;
  rotateXDeg(deg: number): void;
  rotateYDeg(deg: number): void;
  rotateZDeg(deg: number): void;
  enableAlphaBlending(): void;
  disableAlphaBlending(): void;
  enableDepthTest(): void;
  disableDepthTest(): void;
  enableArbTex(): void;
  disableArbTex(): void;

  // --- Input ---
  getMouseX(): number;
  getMouseY(): number;
  getPreviousMouseX(): number;
  getPreviousMouseY(): number;
  getKeyPressed(keyCode: number): boolean;
  getMousePressed(button?: number): boolean;
  sendMessage(msg: string): boolean;

  // --- Application ---
  getWidth(): number;
  getHeight(): number;
  setFrameRate(fps: number): void;
  getFrameRate(): number;
  setWindowTitle(title: string): void;
  setWindowShape(w: number, h: number): void;
  toggleFullscreen(): void;

  // --- Time / Utils ---
  getElapsedTimef(): number;
  getElapsedTimeMillis(): number;
  getElapsedTimeMicros(): number;
  getFrameNum(): number;
  getHours(): number;
  getMinutes(): number;
  getSeconds(): number;
  getMonth(): number;
  getUnixTime(): number;
  getTimestampString(fmt?: string): string;
  getVersionInfo(): string;
  getVersionMajor(): number;
  getVersionMinor(): number;
  getVersionPatch(): number;
  getEnv(varName: string): string;
  map(v: number, i0: number, i1: number, o0: number, o1: number, clamp?: boolean): number;
  clamp(v: number, min: number, max: number): number;
  lerp(a: number, b: number, t: number): number;
  norm(v: number, min: number, max: number): number;

  // --- Classes ---
  Fbo: new () => OfFbo;
  Image: new () => OfImage;
  Shader: new () => OfShader;
  Mesh: new (mode?: OfMeshMode) => OfMesh;
  VideoPlayer: new () => OfVideoPlayer;
  VideoGrabber: new () => OfVideoGrabber;
  SoundPlayer: new () => OfSoundPlayer;
  SoundStream: new () => OfSoundStream;
  Font: new () => OfFont;
  Camera: new () => OfCamera;
  EasyCam: new () => OfEasyCam;
  Serial: new () => OfSerial;
  Vec2: OfVec2Constructor;
  Vec3: OfVec3Constructor;
  Color: OfColorConstructor;
  Rect: OfRectConstructor;
}

declare var of: OfNamespace;

// ── Globals injected by the runtime ──────────────────────────────────────────

/** Absolute path of the directory containing the currently running script. */
declare var __dirname: string;

// ── User-defined callback functions ──────────────────────────────────────────
// Define any of these in your JS file and they will be called automatically.

declare function draw(): void;
declare function update(): void;
declare function mousePressed(x: number, y: number, button: number): void;
declare function mouseReleased(x: number, y: number, button: number): void;
declare function mouseMoved(x: number, y: number): void;
declare function mouseDragged(x: number, y: number, button: number): void;
declare function keyPressed(key: number): void;
declare function keyReleased(key: number): void;
declare function windowResized(w: number, h: number): void;
