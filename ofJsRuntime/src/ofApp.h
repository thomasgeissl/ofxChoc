#pragma once

#include "ofMain.h"
#include "ofxChoc.h"

class ofApp : public ofBaseApp
{

public:
    ofApp();
    ofApp(std::filesystem::path path, bool watch = false);
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void onFileWatcherEvent(choc::file::Watcher::Event &event);

    ofxChoc::JsRuntime _jsRuntime;
    std::filesystem::path _path;
    ofxChoc::FileWatcher *_watcher;

    bool _replMode;
    bool _gameLoopMode;
};
