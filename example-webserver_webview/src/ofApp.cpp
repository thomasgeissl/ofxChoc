#include "ofApp.h"

void ofApp::setup()
{
  _server.setup(1248);
  _speed = 0.5;
  ofAddListener(_webview._event, this, &ofApp::onWebViewEvent);
  _webview.setup("example-webview");
  _webview.navigate("http://localhost:1248/");
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