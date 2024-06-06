#include "ofApp.h"

ofApp::ofApp() : _fileWatcher(ofFilePath::getAbsolutePath("speed.txt", true), 500)
{
}
void ofApp::setup()
{
  _speed = 0.5;

  ofAddListener(
      _fileWatcher._event,
      this, &ofApp::onFileWatcherEvent);
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

void ofApp::onFileWatcherEvent(choc::file::Watcher::Event &event)
{
  ofLogNotice() << "file changed";
  ofFile file(event.file);
  if (file.exists())
  {
    ofBuffer buffer = file.readToBuffer();
    std::string fileContent = buffer.getText();
    ofLog() << "File Content: " << fileContent;
    _speed = ofToFloat(fileContent);
  }
  else
  {
    ofLog() << "File not found: " << event.file;
  }
}
