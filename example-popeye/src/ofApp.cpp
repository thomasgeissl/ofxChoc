#include "ofApp.h"

void ofApp::setup()
{
  _host = "localhost";
  _port = 8000;
  _sender.setup("localhost", 8000);

  // _server.setup(1248, ofToDataPath("webview/dist"));
  ofAddListener(_webview._event, this, &ofApp::onWebViewEvent);
  _webview.setup("popeye");
  _webview.navigate("https://thomasgeissl.github.io/popeye/");
  // _webview.navigate("http://localhost:5175");
  // TODO: serve popeye frontend locally
}

void ofApp::update()
{
}

void ofApp::draw()
{
}

void ofApp::keyPressed(int key)
{
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
  if (event.name == "osc")
  {
    auto value = event.value;
    std::string host = "localhost";
    int port = 8000;
    std::string address = "";
    ofxOscMessage m;

    if (value.contains("host"))
    {
      host = value.at("host").get<std::string>();
    }
    if (value.contains("port"))
    {
      port = value.at("port").get<int>();
    }
    if (value.contains("address"))
    {
      address = value.at("address").get<std::string>();
    }
    if (value.contains("args"))
    {
      for (auto &arg : value.at("args"))
      {
        if (arg.is_number())
        {
          m.addFloatArg(arg.get<float>());
        }
        else if (arg.is_string())
        {
          m.addStringArg(arg.get<std::string>());
        }
      }
    }

    if (host != _host || port != _port)
    {
      _sender.setup(host, port);
      _host = host;
      _port = port;
    }
    m.setAddress(address);
    _sender.sendMessage(m, false);
  }
}