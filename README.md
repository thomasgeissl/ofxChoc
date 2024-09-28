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
* choc: basic usage of choc webview, without openframeworks to debug a bug on linux
