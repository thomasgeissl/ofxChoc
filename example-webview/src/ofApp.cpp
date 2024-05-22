#include "ofApp.h"

void ofApp::setup()
{
    ofAddListener(_webview._event, this, &ofApp::onWebViewEvent);
    _webview.setup();
    _webview.navigate("https://github.com/openFrameworks/openFrameworks");
}

void ofApp::update()
{
}

void ofApp::draw()
{
    ofDrawCircle(ofMap(std::sin(ofGetElapsedTimef()), -1, 1, 0, ofGetWidth()), ofGetHeight() / 2, 10);
}

void ofApp::keyPressed(int key)
{
    switch (key)
    {
    case ' ':
    {
        _webview.setHTML(R"xxx(
      <!DOCTYPE html> <html>
        <head> <title>Page Title</title> </head>
        <script>
          var eventCounter = 0;
          function sendEvent()
          {
            // When you invoke a function, it returns a Promise object
            eventCallbackFn({ counter: ++eventCounter }, "Hello World")
              .then ((result) => { document.getElementById ("eventResultDisplay").innerText = result; });
          }
        </script>

        <body>
          <h1>CHOC WebView Demo</h1>
          <p>This is a demo of a choc::ui::WebView window</p>
          <p><button onclick="sendEvent()">Click to invoke an event callback</button></p>
          <p><input type="file" /></p>
          <p id="eventResultDisplay"></p>
        </body>
      </html>
    )xxx");
        break;
    }
    default:
        break;
    }
}

void ofApp::keyReleased(int key)
{
}

void ofApp::mouseMoved(int x, int y)
{
}

void ofApp::mouseDragged(int x, int y, int button)
{
}

void ofApp::mousePressed(int x, int y, int button)
{
}

void ofApp::mouseReleased(int x, int y, int button)
{
}

void ofApp::mouseEntered(int x, int y)
{
}

void ofApp::mouseExited(int x, int y)
{
}

void ofApp::windowResized(int w, int h)
{
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::onWebViewEvent(ofxChoc::WebView::Event &event){
    ofLogNotice("ofApp::onWebViewEvent") << "got event";
}
