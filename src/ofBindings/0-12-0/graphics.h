
#include "ofMain.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfGraphicsBindings(choc::javascript::Context &context){
    
    context.registerFunction ("_ofxChoc_ofBindings_background",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        if(args.size() == 3){
            ofBackground(args[0]->getFloat64(), args[1]->getFloat64(), args[2]->getFloat64());
        }
        if(args.size() == 4){
            ofBackground(args[0]->getFloat64(), args[1]->getFloat64(), args[2]->getFloat64(), args[3]->getFloat64());
        }
        return context.evaluateExpression("undefined");
    });
    context.registerFunction ("_ofxChoc_ofBindings_drawCircle",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        for(auto arg : args){
            // ofLogNotice() << "arg: " << arg.getType();
        }
        if(args.size() == 3){
            ofDrawCircle(args[0]->getFloat64(), args[1]->getFloat64(), args[2]->getFloat64());
        }
        return context.evaluateExpression("undefined");
    });
    context.registerFunction ("_ofxChoc_ofBindings_drawCurve",
                              [&context] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        // if(args.size() == 8){
        //     ofDrawCurve(args[0]->getFloat64(), args[1]->getFloat64(), args[2]->getFloat64(), args[3]->getFloat64(), args[4]->getFloat64(), args[5]->getFloat64(), args[6]->getFloat64(), args[7]->getFloat64());
        // }
        if(args.size() == 12){
            if(args[9]->isVoid()){
                ofDrawCurve(args[0]->getFloat64(), args[1]->getFloat64(), args[2]->getFloat64(), args[3]->getFloat64(), args[4]->getFloat64(), args[5]->getFloat64(), args[6]->getFloat64(), args[7]->getFloat64());
            }else{
                ofDrawCurve(
                    args[0]->getFloat64(), args[1]->getFloat64(), args[2]->getFloat64(),
                    args[3]->getFloat64(), args[4]->getFloat64(), args[5]->getFloat64(),
                    args[6]->getFloat64(), args[7]->getFloat64(), args[8]->getFloat64(),
                    args[9]->getFloat64(), args[10]->getFloat64(), args[11]->getFloat64()
                );
            }
        }
        return context.evaluateExpression("undefined");
    });
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

    context.run (R"(

ofxChoc_graphics = {
    background: function(r,g,b,a=255) { return _ofxChoc_ofBindings_background(r,g,b,a) },
    drawCircle: function(x,y,r) { return _ofxChoc_ofBindings_drawCircle(x,y,r) },
    drawCurve: function(x0,y0,x1,y1,x2,y2,x3,y3,z0=null, z1=null, z2=null, z3=null) { return _ofxChoc_ofBindings_drawCircle(x0,y0,x1,y1,x2,y2,x3,y3, z0, z1, z2, z3) },
    setColor: function(r,g,b,a=255) { return _ofxChoc_ofBindings_setColor(r,g,b,a) },
};

)");

}