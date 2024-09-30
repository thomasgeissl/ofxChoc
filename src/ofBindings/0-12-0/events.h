
#pragma once
#include "ofMain.h"
#include <tuple>
#include <utility>
#include <type_traits>
#include "../helpers/quickjs.h"
#include "../helpers/webview.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"


inline auto getKeyPressedLambda = [](int key) -> bool {
    return ofGetKeyPressed(key);
};
inline auto getMouseXLambda = []() -> int {
    return ofGetMouseX();
};
inline auto getMouseYLambda = []() -> int {
    return ofGetMouseY();
};

inline void registerOfEventsBindings(choc::ui::WebView *webview)
{
    // bindFunction(webview, "_ofxChoc_ofBindings_getKeyPressed", {"int"}, "bool", getKeyPressedLambda);
    // bindFunction(webview, "_ofxChoc_ofBindings_getMousePressed", {"float"}, "bool", ofGetMousePressed);
    bindFunction(webview, "_ofxChoc_ofBindings_getMouseX", {}, "int", getMouseXLambda);
    bindFunction(webview, "_ofxChoc_ofBindings_getMouseY", {}, "int", getMouseYLambda);
    // bindFunction(webview, "_ofxChoc_ofBindings_getPreviousMouseX", {}, "float", ofGetPreviousMouseX);
    // bindFunction(webview, "_ofxChoc_ofBindings_getPreviousMouseY", {}, "float", ofGetPreviousMouseY);
    // bindFunction(webview, "_ofxChoc_ofBindings_sendMessage", {"string"}, "undefined", ofSendMessage);
    // bindFunction(webview, "_ofxChoc_ofBindings_sendMessage", {"string"}, "undefined", 
            //  [](const std::string &message) { ofSendMessage(message); });

    

    webview->evaluateJavascript(R"(
        ofxChoc_events = {
            getKeyPressed: function(keyCode) { return _ofxChoc_ofBindings_getKeyPressed(keyCode); },
            getMousePressed: function(button) { return _ofxChoc_ofBindings_getMousePressed(button); },
            getMouseX: function() { return _ofxChoc_ofBindings_getMouseX(); },
            getMouseY: function() { return _ofxChoc_ofBindings_getMouseY(); },
            getPreviousMouseX: function() { return _ofxChoc_ofBindings_getPreviousMouseX(); },
            getPreviousMouseY: function() { return _ofxChoc_ofBindings_getPreviousMouseY(); },
            // sendMessage: function(message) { return _ofxChoc_ofBindings_sendMessage(message); },
        };
    )");
}

inline void registerOfEventsBindings(choc::javascript::Context &context)
{

    // context.registerFunction("_ofxChoc_ofBindings_getKeyPressed",
    //                          [&context](choc::javascript::ArgumentList args) mutable -> choc::value::Value
    //                          {
    //                              if (args.size() == 1)
    //                              {
    //                                  auto key = args[0]->getFloat64();
    //                                  return choc::value::createBool(ofGetKeyPressed(key));
    //                              }
    //                              else
    //                              {
    //                                  return choc::value::createBool(false);
    //                              }
    //                          });
    context.registerFunction("_ofxChoc_ofBindings_getMousePressed",
                             [&context](choc::javascript::ArgumentList args) mutable -> choc::value::Value
                             {
                                 auto button = 0;
                                 if (args.size() == 1)
                                 {
                                     button = args[0]->getFloat64();
                                 }
                                 return choc::value::createBool(ofGetMousePressed(button));
                             });
    // context.registerFunction("_ofxChoc_ofBindings_getMouseX",
    //                          [&context](choc::javascript::ArgumentList args) mutable -> choc::value::Value
    //                          {
    //                              return choc::value::createFloat64(ofGetMouseX());
    //                          });

    // context.registerFunction("_ofxChoc_ofBindings_getMouseY",
    //                          [&context](choc::javascript::ArgumentList args) mutable -> choc::value::Value
    //                          {
    //                              return choc::value::createFloat64(ofGetMouseY());
    //                          });
// bindFunction(context, "_ofxChoc_ofBindings_getKeyPressed", {"int"}, "bool", getKeyPressedLambda);
// bindFunction(context, "_ofxChoc_ofBindings_getKeyPressed", {}, getKeyPressedLambda);
bindFunction(context, "_ofxChoc_ofBindings_getMouseX", {}, getMouseXLambda);
bindFunction(context, "_ofxChoc_ofBindings_getMouseY", {}, getMouseXLambda);


    context.registerFunction("_ofxChoc_ofBindings_getPreviousMouseX",
                             [&context](choc::javascript::ArgumentList args) mutable -> choc::value::Value
                             {
                                 return choc::value::createFloat64(ofGetPreviousMouseX());
                             });
    context.registerFunction("_ofxChoc_ofBindings_getPreviousMouseY",
                             [&context](choc::javascript::ArgumentList args) mutable -> choc::value::Value
                             {
                                 return choc::value::createFloat64(ofGetPreviousMouseY());
                             });
    context.registerFunction("_ofxChoc_ofBindings_sendMessage",
                             [&context](choc::javascript::ArgumentList args) mutable -> choc::value::Value
                             {
                                 std::string message = "";
                                 if (args.size() == 1)
                                 {
                                     message = args[0]->getString();
                                 }
                                 ofSendMessage(message);
                                 return context.evaluateExpression("undefined");
                             });

    context.run(R"(

ofxChoc_events = {
    getKeyPressed: function(keyCode) { return _ofxChoc_ofBindings_getKeyPressed(keyCode) },
    getMousePressed: function(button) { return _ofxChoc_ofBindings_getMousePressed(button) },
    getMouseX: function() { return _ofxChoc_ofBindings_getMouseX() },
    getMouseY: function() { return _ofxChoc_ofBindings_getMouseY() },
    getPreviousMouseX: function() { return _ofxChoc_ofBindings_getPreviousMouseX() },
    getPreviousMouseY: function() { return _ofxChoc_ofBindings_getPreviousMouseY() },
    sendMessage: function(message) { return _ofxChoc_ofBindings_sendMessage(message) },
};

)");
}