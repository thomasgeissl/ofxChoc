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
            <h5>press 1,2,3,4,5,6 to check the file dialogs, they are not yet exposed to the js api. for now, they can only be used on the cpp side, but hey at least they are non blocking. working on it ...</h5>
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
  case '1':
  {

    _dialogs.messageBox("Information", "This is an information message", "ok", [&]()
                        { ofLogNotice() << "Message box closed."; });

    break;
  }
  case '2':
  {
    _dialogs.openFileDialog("Open a file", "", "*.txt", [&](const std::string &filePath)
                            {
            if (!filePath.empty()) {
                ofLogNotice() << "Selected file: " << filePath;
            } else {
                ofLogNotice() << "No file selected.";
            } });

    break;
  }
  case '3':
  {

    _dialogs.saveFileDialog("Save a file", "", "*.txt", [&](const std::string &filePath)
                            {
            if (!filePath.empty()) {
                ofLogNotice() << "File saved as: " << filePath;
            } else {
                ofLogNotice() << "No file saved.";
            } });
    break;
  }
  case '4':
  {

    _dialogs.chooseFolderDialog("Select a folder", "", [&](const std::string &folderPath)
                                {
            if (!folderPath.empty()) {
                ofLogNotice() << "Selected folder: " << folderPath;
            } else {
                ofLogNotice() << "No folder selected.";
            } });
    break;
  }
  case '5':
  {

    _dialogs.inputBox("Input Box", "Enter some text", "default text", [&](const std::string &input)
                      { ofLogNotice() << "Entered text: " << input; });

    break;
  }
  case '6':
  {

    ofColor defaultColor(255, 0, 0);
    _dialogs.colorPickerDialog("Pick a color", defaultColor, [&](const ofColor &color)
                               {
                                 ofLogNotice() << "Enter color: " << color;
                                 // TODO: why is it blocking?
                                 // guess trying to draw from not the main thread is not a smart idea
                                 // ofSetBackgroundColor(color);
                               });

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
  }
  else
  {
    ofLogNotice() << "dom event: " << event.selector << " " << event.eventName << " " << event.value.dump(2);
  }
}