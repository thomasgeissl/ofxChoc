#include "ofApp.h"

void ofApp::setup()
{
  _speed = 0.5;
  ofAddListener(_webview._event, this, &ofApp::onWebViewEvent);
  _webview.setup();
  _webview.setHTML(R"xxx(
      <!DOCTYPE html> <html>
        <head> <title>Page Title</title> </head>
        <script nonce="1234"> 
          function sendEvent()
          {
            eventCallbackFn({ event: "randombg"})
              .then ((result) => { console.log(result); });
          }

          document.addEventListener('DOMContentLoaded', (event) => {
            const slider = document.getElementById("slider");
            const sliderValue = document.getElementById("sliderValue");

            function sliderCallback(value) {
                console.log("Slider value changed to: " + value);
                eventCallbackFn({ slider: value})
            }

            slider.oninput = function() {
                sliderValue.innerText = this.value;
                sliderCallback(this.value);
            }

            window.of.addListener("testevent", (value) => {
              document.getElementById("from-of").textContent = JSON.stringify(value);
              console.log("test event", value);
            })
        });
        </script>

        <body>
          <input type="range" id="slider" name="slider" min="0" max="1" step="0.01" value="0.5">
          <p>Value: <span id="sliderValue">0.5</span></p>
          <p><button onclick="sendEvent()">random bg</button></p>
          <div>
            <h5>press e on the of side</h5>
            <p id="from-of"></p>
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
  for (auto item : event.value)
  {
    if (item.contains("slider"))
    {
      _speed = ofToFloat(item.at("slider").get<std::string>());
    }
    if (item.contains("event"))
    {
      auto name = item.at("event").get<std::string>();
      if (name == "randombg")
      {
        ofBackground(ofRandom(255), ofRandom(255), ofRandom(255));
      }
    }
  }
}