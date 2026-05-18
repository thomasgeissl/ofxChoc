#pragma once

#include "ofMain.h"
#include "ofxChoc.h"

class ofApp : public ofBaseApp
{
public:
    ofApp();

    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    // Called from JS to push a message into the log
    void addMessage(std::string msg);

    choc::javascript::Context _context;
    std::vector<std::string>  _messages;
};
