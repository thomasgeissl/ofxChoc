
#include "ofMain.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfGlBindings(choc::javascript::Context &context){
    
    context.registerFunction ("_ofxChoc_ofBindings_disableArbTex",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        ofDisableArbTex();
        return context.evaluateExpression("undefined");
    });
    
    context.run (R"(

ofxChoc_gl = {
    disableArbTex: function() { return _ofxChoc_ofBindings_disableArbTex() },
};

)");

}