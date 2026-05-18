#include "ofApp.h"
#include "ofBindings/0-12-1/helpers.h"

ofApp::ofApp() {}

void ofApp::addMessage(std::string msg)
{
    _messages.push_back(msg);
}

void ofApp::setup()
{
    _context = choc::javascript::createQuickJSContext();
    choc::javascript::registerConsoleFunctions(_context);

    bindFn(_context, "_app_addMessage", this, &ofApp::addMessage);

    _context.run("var app = { addMessage: function(msg) { return _app_addMessage(msg); } };");

    ofFile file(ofToDataPath("sketch.js"));
    if (!file.exists()) { ofLogError("example-custombindings") << "sketch.js not found in bin/data/"; return; }
    _context.evaluateExpression(ofBuffer(file.readToBuffer()).getText());
}

void ofApp::update() {}

void ofApp::draw()
{
    ofBackground(20, 24, 36);

    // instructions
    ofSetColor(180, 180, 180);
    ofDrawBitmapString("press SPACE to call greet() in JS", 20, 30);

    // message log
    int y = 70;
    for (auto& msg : _messages) {
        ofSetColor(100, 220, 160);
        ofDrawBitmapString(msg, 20, y);
        y += 18;
    }
}

void ofApp::keyPressed(int key)
{
    if (key == ' ') {
        try { _context.evaluateExpression("greet()"); }
        catch (choc::javascript::Error& e) { ofLogError("example-custombindings") << e.what(); }
    }
}
