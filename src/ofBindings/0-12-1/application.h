#pragma once

#include "ofMain.h"
#include "helpers.h"

inline void registerOfApplicationBindings(choc::javascript::Context &context)
{
    bindFn(context, "_ofxChoc_ofBindings_getHeight",        &ofGetHeight);
    bindFn(context, "_ofxChoc_ofBindings_getWidth",         &ofGetWidth);
    bindFn(context, "_ofxChoc_ofBindings_toggleFullscreen", &ofToggleFullscreen);
    bindFn(context, "_ofxChoc_ofBindings_getFrameRate",     &ofGetFrameRate);
    bindFn(context, "_ofxChoc_ofBindings_setFrameRate",     &ofSetFrameRate);
    bindFn(context, "_ofxChoc_ofBindings_setWindowTitle",   &ofSetWindowTitle);
    bindFn(context, "_ofxChoc_ofBindings_setWindowShape",   &ofSetWindowShape);

    context.run(R"(
ofxChoc_application = {
    getHeight:        function()           { return _ofxChoc_ofBindings_getHeight() },
    getWidth:         function()           { return _ofxChoc_ofBindings_getWidth() },
    toggleFullscreen: function()           { return _ofxChoc_ofBindings_toggleFullscreen() },
    getFrameRate:     function()           { return _ofxChoc_ofBindings_getFrameRate() },
    setFrameRate:     function(fps)        { return _ofxChoc_ofBindings_setFrameRate(fps) },
    setWindowTitle:   function(title)      { return _ofxChoc_ofBindings_setWindowTitle(title) },
    setWindowShape:   function(w,h)        { return _ofxChoc_ofBindings_setWindowShape(w,h) },
};
)");
}
