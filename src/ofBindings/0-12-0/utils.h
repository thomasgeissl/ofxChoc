
#include "ofMain.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfUtilsBindings(choc::javascript::Context &context){
    
    context.registerFunction ("_ofxChoc_ofBindings_getElapsedTimeMicros",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetElapsedTimeMicros());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getElapsedTimeMillis",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetElapsedTimeMillis());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getElapsedTimef",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetElapsedTimef());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getEnv",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        std::string var = "";
        if(args.size() == 1){
            var = args[0]->getString();
        }
        return choc::value::createString(ofGetEnv(var));
    });
    context.registerFunction ("_ofxChoc_ofBindings_getFrameNum",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetFrameNum());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getHours",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetHours());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getMinutes",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetMinutes());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getMonth",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetMonth());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getSeconds",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetSeconds());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getSystemTimeMicros",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(1);
        return choc::value::createFloat64((float) ofGetSystemTimeMicros());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getSystemTimeMillis",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(1);
        return choc::value::createFloat64((float) ofGetSystemTimeMillis());
    });
    //TODO: ofGetTargetPlatform
    context.registerFunction ("_ofxChoc_ofBindings_getTimestampString",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        std::string timestampFormat = "";
        if(args.size() == 1){
            timestampFormat = args[0]->getString();
        }
        return choc::value::createString(ofGetTimestampString(timestampFormat));
    });
    context.registerFunction ("_ofxChoc_ofBindings_getUnixTime",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetUnixTime());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getVersionInfo",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createString(ofGetVersionInfo());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getVersionMajor",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetVersionMajor());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getVersionMinor",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetVersionMinor());
    });
    context.registerFunction ("_ofxChoc_ofBindings_getVersionPatch",
                              [] (choc::javascript::ArgumentList args) mutable -> choc::value::Value
    {
        return choc::value::createFloat64(ofGetVersionPatch());
    });

try {
    context.run (R"(

ofxChoc_utils = {
    getElapsedTimeMicros: function() { return _ofxChoc_ofBindings_getElapsedTimeMicros() },
    getElapsedTimeMillis: function() { return _ofxChoc_ofBindings_getElapsedTimeMillis() },
    getElapsedTimef: function() { return _ofxChoc_ofBindings_getElapsedTimef() },
    // getEnv: function(var) { return _ofxChoc_ofBindings_getEnv(var)},
    getFrameNum: function() { return _ofxChoc_ofBindings_getFrameNum() },
    getHours: function() { return _ofxChoc_ofBindings_getHours() },
    getMinutes: function() { return _ofxChoc_ofBindings_getMinutes() },
    getMonth: function() { return _ofxChoc_ofBindings_getMonth() },
    getSeconds: function() { return _ofxChoc_ofBindings_getSeconds() },
    // getSystemTimeMicros: function() { return _ofxChoc_getSystemTimeMicros() },
    // getSystemTimeMillis: function() { return _ofxChoc_getSystemTimeMillis() },
    // getTimestampString: function(timestampFormat="") { return _ofxChoc_getTimestampString(timestampFormat) },
    // getUnixTime: function() { return _ofxChoc_getUnixTime() },
    // getVersionInfo: function() { return _ofxChoc_getVersionInfo() },
    // getVersionMajor: function() { return _ofxChoc_getVersionMajor() },
    // getVersionMinor: function() { return _ofxChoc_getVersionMinor() },
    // getVersionPatch: function() { return _ofxChoc_getVersionPatch() },
};

)");
}catch(choc::javascript::Error &e){
    ofLogError() << e.what();
}

}