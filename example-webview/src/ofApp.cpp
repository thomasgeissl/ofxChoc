#include "ofApp.h"

void ofApp::setup()
{
  _speed = 0.5;
  ofAddListener(_webview._event, this, &ofApp::onWebViewEvent);
  ofAddListener(_webview._domEvent, this, &ofApp::onWebViewDomEvent);
  _webview.setup("example-webview");
  _webview.setHTML(R"xxx(
      <!DOCTYPE html> <html>
        <head> <title>ofxChoc::example-webview</title> </head>
        <script> 
          function buttonCallback()
          {
                window.ofxChoc.notifyEvent("randombg")
          }

          document.addEventListener('DOMContentLoaded', (event) => {
            const slider = document.getElementById("slider");
            const sliderValue = document.getElementById("sliderValue");

            function sliderCallback(value) {
                console.log("Slider value changed to: " + value);
                window.ofxChoc.notifyEvent("slider", {"value": value})
            }

            slider.oninput = function() {
                sliderValue.innerText = this.value;
                sliderCallback(this.value);
            }

            window.ofxChoc.addListener("testevent", (value) => {
              document.getElementById("from-of").textContent = JSON.stringify(value);
              console.log("test event", value);
            })
            window.ofxChoc.addListener("testevent", (value) => {
              console.log("test event, second listener on the same event, just to test if that works", value);
            })
        });
        </script>

        <body>
          <input type="range" id="slider" name="slider" min="0" max="1" step="0.01" value="0.5">
          <p>Value: <span id="sliderValue">0.5</span></p>
          <p><button id="button" onclick="buttonCallback()">random bg</button></p>
          <div>
            <h5>press e on the of side to get a random value generated in oF</h5>
            <p id="from-of"></p>
            <h5>press d on the of side to register a dom event handler, hover over the button and see the ofLogs</h5>
          </div>
        </body>
      </html>
    )xxx");
}

void ofApp::update()
{
}

void ofApp::draw()
{
  ofDrawCircle(
      ofMap(std::sin(ofGetElapsedTimef() * _speed * 8), -1, 1, 0, ofGetWidth()),
      ofGetHeight() / 2,
      54);
}

void ofApp::keyPressed(int key)
{
  switch (key)
  {
  case 'd':
  {
    _webview.addDomListener("window", "loaded");
    _webview.addDomListener("document", "mousemove");
    _webview.addDomListener("#button", "mouseenter");
    break;
  }
  case 'e':
  {
    auto json = ofJson::parse(R"({})");
    json["value"] = ofRandom(0, 1);
    _webview.notifyEvent("testevent", json);
    break;
  }
  case ' ':
  {
    _webview.navigate("https://github.com/openFrameworks/openFrameworks");
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

void ofApp::onWebViewEvent(ofxChoc::WebView::Event &event)
{
  if (event.name == "slider")
  {
    for (auto item : event.value)
    {
      if (item.contains("value"))
      {
        _speed = ofToFloat(item.at("value").get<std::string>());
      }
    }
  }
  if (event.name == "randombg")
  {
    ofBackground(ofRandom(255), ofRandom(255), ofRandom(255));
  }
}

void ofApp::onWebViewDomEvent(ofxChoc::WebView::DomEvent &event)
{
  if (event.selector == "#button")
  {
    ofLogNotice() << "dom event: " << event.selector << " " << event.eventName << " " << event.value.dump(2);
  }else{
    ofLogNotice() << "dom event: " << event.selector << " " << event.eventName << " " << event.value.dump(2);
  }
}