#include "ofApp.h"

void ofApp::setup()
{
  _server.setup(1248, ofToDataPath("webview/dist"));
  _speed = 0.5;
  ofAddListener(_webview._event, this, &ofApp::onWebViewEvent);
  _webview.setup("example-webview");
  _webview.navigate("http://localhost:1248/");
  // _webview.navigate("http://localhost:5173/");

  _fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
}

void ofApp::update()
{
  ofPixels pixels;
  _fbo.readToPixels(pixels);
  ofBuffer buffer;
  ofSaveImage(pixels, buffer, OF_IMAGE_FORMAT_PNG);
  std::string encodedData = ofxChoc::utils::base64_encode(reinterpret_cast<const unsigned char*>(buffer.getData()), buffer.size());
  auto json = ofJson::parse(R"({})");
  json["data"] = encodedData;
  json["width"] = _fbo.getWidth();
  json["height"] = _fbo.getHeight();
  _webview.notifyEvent("texture", json);
}

void ofApp::draw()
{
  _fbo.begin();
  ofClear(0, 0, 0, 0);
  ofSetColor(ofColor::orange);
  ofDrawCircle(
      ofMap(std::sin(ofGetElapsedTimef() * _speed * 8), -1, 1, 0, ofGetWidth()),
      ofGetHeight() / 2,
      54);
  _fbo.end();
  _fbo.draw(0, 0);
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