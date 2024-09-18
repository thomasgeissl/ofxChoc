
#include "ofMain.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfGlBindings(choc::javascript::Context &context){
    
    context.registerFunction ("_ofxChoc_ofBindings_setColor",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        if(args.size() == 3){
            ofSetColor(args[0]->getFloat64(), args[1]->getFloat64(), args[2]->getFloat64());
        }
        if(args.size() == 4){
            ofSetColor(args[0]->getFloat64(), args[1]->getFloat64(), args[2]->getFloat64(), args[3]->getFloat64());
        }
        return context.evaluateExpression("undefined");
    });
    context.registerFunction ("_ofxChoc_ofBindings_drawCircle",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        ofLogNotice() << "draw cirlce" << args.size();
        for(auto arg : args){
            // ofLogNotice() << "arg: " << arg.getType();
        }
        if(args.size() == 3){
            ofDrawCircle(args[0]->getFloat64(), args[1]->getFloat64(), args[2]->getFloat64());
        }
        return context.evaluateExpression("undefined");
    });
    context.run (R"(

ofxChoc_gl = {
    setColor: function(r,g,b,a=255) { return _ofxChoc_ofBindings_setColor(r,g,b,a) },
    drawCircle: function(x,y,r) { return _ofxChoc_ofBindings_drawCircle(x,y,r) },
};

)");

}