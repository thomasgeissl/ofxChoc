#include "ofApp.h"

ofApp::ofApp() : _watcher(nullptr), _replMode(true), _gameLoopMode(false)
{
    ofLogNotice("ofJsRuntime") << "starting in repl mode";
}
ofApp::ofApp(std::filesystem::path path, bool gameLoopActive, bool watch) : _path(path), _watcher(new ofxChoc::FileWatcher(path)), _replMode(false), _gameLoopMode(true)
{
  if(watch){
    ofAddListener(
        _watcher->_event,
        this, &ofApp::onFileWatcherEvent);
  }
  ofLogNotice("ofJsRuntime") << "starting in file mode";
  _jsRuntime.evaluateFile(_path, gameLoopActive);
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
  _jsRuntime.onKeyPressed(key);
}

void ofApp::keyReleased(int key)
{
  _jsRuntime.onKeyReleased(key);
}

void ofApp::mouseMoved(int x, int y)
{
  _jsRuntime.onMouseMoved(x, y);
}

void ofApp::mouseDragged(int x, int y, int button)
{
  _jsRuntime.onMouseDragged(x, y, button);
}

void ofApp::mousePressed(int x, int y, int button)
{
  _jsRuntime.onMousePressed(x, y, button);
}

void ofApp::mouseReleased(int x, int y, int button)
{
  _jsRuntime.onMouseReleased(x, y, button);
}

void ofApp::mouseEntered(int x, int y)
{
  _jsRuntime.onMouseEntered(x, y);
}

void ofApp::mouseExited(int x, int y)
{
  _jsRuntime.onMouseExited(x, y);
}

void ofApp::windowResized(int w, int h)
{
  _jsRuntime.onWindowResized(w, h);
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
  _jsRuntime.onDragEvent(dragInfo);
}

void ofApp::gotMessage(ofMessage msg)
{
  _jsRuntime.onMessage(msg);
}

void ofApp::onFileWatcherEvent(choc::file::Watcher::Event &event)
{
  _jsRuntime.evaluateFile(_path, _gameLoopMode);
}