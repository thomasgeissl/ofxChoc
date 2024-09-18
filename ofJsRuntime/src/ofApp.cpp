#include "ofApp.h"

ofApp::ofApp() : _watcher(nullptr), _replMode(true), _gameLoopMode(false)
{
    ofLogNotice("ofJsRuntime") << "starting in repl mode";
}
ofApp::ofApp(std::filesystem::path path, bool watch) : _path(path), _watcher(new ofxChoc::FileWatcher(path)), _replMode(false), _gameLoopMode(true)
{
  if(watch){
    ofAddListener(
        _watcher->_event,
        this, &ofApp::onFileWatcherEvent);
  }
  ofLogNotice("ofJsRuntime") << "starting in file mode";
}
void ofApp::setup()
{
  _jsRuntime.setup();
  if(_replMode){
    _jsRuntime.startRepl();
  }
  if(_gameLoopMode){
    _jsRuntime.startGameLoop();
  }
}

void ofApp::update()
{
  _jsRuntime.update();
}

void ofApp::draw()
{
  _jsRuntime.draw();
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
  }
  else
  {
    ofLog() << "File not found: " << event.file;
  }
}