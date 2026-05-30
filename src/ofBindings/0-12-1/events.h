#pragma once

#include "ofMain.h"
#include "helpers.h"

inline void registerOfEventsBindings(choc::javascript::Context &context)
{
    bindFn(context, "_ofxChoc_ofBindings_getMouseX",         &ofGetMouseX);
    bindFn(context, "_ofxChoc_ofBindings_getMouseY",         &ofGetMouseY);
    bindFn(context, "_ofxChoc_ofBindings_getPreviousMouseX", &ofGetPreviousMouseX);
    bindFn(context, "_ofxChoc_ofBindings_getPreviousMouseY", &ofGetPreviousMouseY);
    // ofSendMessage is overloaded — pick the string variant
    bindFn(context, "_ofxChoc_ofBindings_sendMessage",
           static_cast<bool(*)(std::string)>(&ofSendMessage));

    bindFn(context, "_ofxChoc_ofBindings_getKeyPressed",   &ofGetKeyPressed);
    bindFn(context, "_ofxChoc_ofBindings_getMousePressed", &ofGetMousePressed);

    context.run(R"(
ofxChoc_events = {
    getKeyPressed:    function(keyCode)  { return _ofxChoc_ofBindings_getKeyPressed(keyCode) },
    getMousePressed:  function(button=0) { return _ofxChoc_ofBindings_getMousePressed(button) },
    getMouseX:        function()         { return _ofxChoc_ofBindings_getMouseX() },
    getMouseY:        function()         { return _ofxChoc_ofBindings_getMouseY() },
    getPreviousMouseX:function()         { return _ofxChoc_ofBindings_getPreviousMouseX() },
    getPreviousMouseY:function()         { return _ofxChoc_ofBindings_getPreviousMouseY() },
    sendMessage:      function(msg)      { return _ofxChoc_ofBindings_sendMessage(msg) },
};
)");
}
