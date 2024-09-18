
#include "ofMain.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfApplicationBindings(choc::javascript::Context &context){
    
    context.registerFunction ("_ofxChoc_ofBindings_getHeight",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetHeight());
    });

    context.registerFunction ("_ofxChoc_ofBindings_getWidth",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetWidth());
    });

    context.registerFunction ("_ofxChoc_ofBindings_toggleFullscreen",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        ofToggleFullscreen();
        // TODO: how can i return undefined
        return context.evaluateExpression("undefined");
    });


    context.run (R"(

of = {
    getHeight: function() { return _ofxChoc_ofBindings_getHeight() },
    getWidth: function() { return _ofxChoc_ofBindings_getWidth() },
    toggleFullscreen: function() { return _ofxChoc_ofBindings_toggleFullscreen() },
};

)");

}