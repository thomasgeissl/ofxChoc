
#include "ofMain.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfEventsBindings(choc::javascript::Context &context){
    
    context.registerFunction ("_ofxChoc_ofBindings_getKeyPressed",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        if(args.size() == 1){
            auto key = args[0]->getFloat64();
            return choc::value::createBool(ofGetKeyPressed(key));
        }else{
            return choc::value::createBool(false);
        }
    });
    context.registerFunction ("_ofxChoc_ofBindings_getMousePressed",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        auto button = 0;
        if(args.size() == 1){
            button = args[0]->getFloat64();
        }
        return choc::value::createBool(ofGetMousePressed(button));
    });
    context.registerFunction ("_ofxChoc_ofBindings_getMouseX",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetMouseX());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getMouseY",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetMouseY());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getPreviousMouseX",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetPreviousMouseX());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getPreviousMouseY",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetPreviousMouseY());
    });
    context.registerFunction ("_ofxChoc_ofBindings_sendMessage",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        std::string message = "";
        if(args.size() == 1){
            message = args[0]->getString();
        }
        ofSendMessage(message);
        return context.evaluateExpression("undefined");
    });

    context.run (R"(

ofxChoc_events = {
    getKeyPressed: function() { return _ofxChoc_ofBindings_getKeyPressed() },
    getMousePressed: function(button) { return _ofxChoc_ofBindings_getMousePressed(button) },
    getMouseX: function() { return _ofxChoc_ofBindings_getMouseX() },
    getMouseY: function() { return _ofxChoc_ofBindings_getMouseY() },
    getPreviousMouseX: function() { return _ofxChoc_ofBindings_getPreviousMouseX() },
    getPreviousMouseY: function() { return _ofxChoc_ofBindings_getPreviousMouseY() },
    sendMessage: function(message) { return _ofxChoc_ofBindings_sendMessage(message) },
};

)");

}