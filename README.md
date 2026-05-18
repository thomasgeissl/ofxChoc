# ofxChoc

## Description
* this is a work in progress, currently more a proof of concept, still bugs and not yet stable
* this addon comes with the following features
    * webview
    * webserver
    * oF js runtime (wip)
    * filewatcher
    * non-blocking file and message dialogs
    * ... more to come

## Usage
* cd path/to/openFrameworks/addons
* git clone https://github.com/thomasgeissl/ofxChoc
* on linux: `sudo apt install libwebkit2gtk-4.0-dev`
* cd ofxChoc/example-webview
* make -j8 && make run

## Examples
This addon comes with a couple of example
* example-webview: simple webview, html source set at runtime, no webserver
* example-webserver_webview: webview and webserver
* example-filewatcher: watches local files or directories
* ofJsRuntime: oF app that executes JavaScript code, it comes with a couple of oF bindings (wip)
* example-custombindings: minimal example showing how to call a JS function from C++ and expose a C++ member function to JS
* choc: basic usage of choc webview, without openframeworks to debug a bug on linux

## JS Runtime

The JS runtime embeds [QuickJS](https://bellard.org/quickjs/) via choc and exposes openFrameworks as a `of.*` API to JavaScript. Scripts are hot-reloadable and can drive the full oF render loop.

### Built-in `of.*` bindings
A subset of the oF API is available out of the box, e.g. `of.getWidth()`, `of.background()`, `of.drawCircle()`, `new of.Fbo()`, `new of.Shader()`, `new of.Vec2()`. See `src/ofBindings/` for the full list.

`examples-js/` includes an `of.d.ts` type definitions file and a `jsconfig.json` — open the folder in VS Code to get autocompletion for the `of.*` API.

### Custom bindings
Expose any C++ free function with automatic type marshalling:
```cpp
bindFn(context, "jsName", &myCppFunction);
```
Expose a member function by passing the object pointer:
```cpp
bindFn(context, "jsName", this, &MyClass::myMethod);
```
See `example-custombindings` for a minimal working example.

### Running a script
```bash
cd ofJsRuntime
make -j8 && ./bin/ofJsRuntime -f ../examples-js/follow-mouse.js --watch
```
Pass `--watch` to hot-reload the script on every save.
