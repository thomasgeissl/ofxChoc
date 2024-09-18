
#include "ofMain.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfUtilsBindings(choc::javascript::Context &context){
    
    context.registerFunction ("_ofxChoc_ofBindings_getFrameNum",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetFrameNum());
    });

    context.run (R"(

ofxChoc_utils = {
    getFrameNum: function() { return _ofxChoc_ofBindings_getFrameNum() },
};

)");

}