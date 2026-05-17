#pragma once

#include "ofMain.h"
#include "helpers.h"

inline void registerOfUtilsBindings(choc::javascript::Context &context)
{
    bindFn(context, "_ofxChoc_ofBindings_getElapsedTimeMicros", &ofGetElapsedTimeMicros);
    bindFn(context, "_ofxChoc_ofBindings_getElapsedTimeMillis", &ofGetElapsedTimeMillis);
    bindFn(context, "_ofxChoc_ofBindings_getElapsedTimef",      &ofGetElapsedTimef);
    bindFn(context, "_ofxChoc_ofBindings_getFrameNum",          &ofGetFrameNum);
    bindFn(context, "_ofxChoc_ofBindings_getHours",             &ofGetHours);
    bindFn(context, "_ofxChoc_ofBindings_getMinutes",           &ofGetMinutes);
    bindFn(context, "_ofxChoc_ofBindings_getMonth",             &ofGetMonth);
    bindFn(context, "_ofxChoc_ofBindings_getSeconds",           &ofGetSeconds);
    bindFn(context, "_ofxChoc_ofBindings_getUnixTime",          &ofGetUnixTime);
    bindFn(context, "_ofxChoc_ofBindings_getVersionInfo",       &ofGetVersionInfo);
    bindFn(context, "_ofxChoc_ofBindings_getVersionMajor",      &ofGetVersionMajor);
    bindFn(context, "_ofxChoc_ofBindings_getVersionMinor",      &ofGetVersionMinor);
    bindFn(context, "_ofxChoc_ofBindings_getVersionPatch",      &ofGetVersionPatch);

    // ofGetEnv and ofGetTimestampString take a string arg — still use bindFn
    bindFn(context, "_ofxChoc_ofBindings_getEnv",               &ofGetEnv);
    // ofMap: (value, inputMin, inputMax, outputMin, outputMax, clamp=false)
    context.registerFunction("_ofxChoc_ofBindings_map",
        [](choc::javascript::ArgumentList args) -> choc::value::Value {
            bool clamp = args.size() >= 6 ? args[5]->getBool() : false;
            return choc::value::createFloat64(
                ofMap(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2]),
                      fromJS<float>(args[3]), fromJS<float>(args[4]), clamp));
        });
    context.registerFunction("_ofxChoc_ofBindings_clamp",
        [](choc::javascript::ArgumentList args) -> choc::value::Value {
            return choc::value::createFloat64(
                ofClamp(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2])));
        });
    context.registerFunction("_ofxChoc_ofBindings_lerp",
        [](choc::javascript::ArgumentList args) -> choc::value::Value {
            return choc::value::createFloat64(
                ofLerp(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2])));
        });
    context.registerFunction("_ofxChoc_ofBindings_norm",
        [](choc::javascript::ArgumentList args) -> choc::value::Value {
            return choc::value::createFloat64(
                ofNormalize(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2])));
        });
    // ofGetTimestampString is overloaded — pick the single-string-arg variant
    bindFn(context, "_ofxChoc_ofBindings_getTimestampString",
           static_cast<std::string(*)(const std::string&)>(&ofGetTimestampString));

    try {
        context.run(R"(
ofxChoc_utils = {
    getElapsedTimeMicros: function()               { return _ofxChoc_ofBindings_getElapsedTimeMicros() },
    getElapsedTimeMillis: function()               { return _ofxChoc_ofBindings_getElapsedTimeMillis() },
    getElapsedTimef:      function()               { return _ofxChoc_ofBindings_getElapsedTimef() },
    getEnv:               function(varName)        { return _ofxChoc_ofBindings_getEnv(varName) },
    getFrameNum:          function()               { return _ofxChoc_ofBindings_getFrameNum() },
    getHours:             function()               { return _ofxChoc_ofBindings_getHours() },
    getMinutes:           function()               { return _ofxChoc_ofBindings_getMinutes() },
    getMonth:             function()               { return _ofxChoc_ofBindings_getMonth() },
    getSeconds:           function()               { return _ofxChoc_ofBindings_getSeconds() },
    getUnixTime:          function()               { return _ofxChoc_ofBindings_getUnixTime() },
    getVersionInfo:       function()               { return _ofxChoc_ofBindings_getVersionInfo() },
    getVersionMajor:      function()               { return _ofxChoc_ofBindings_getVersionMajor() },
    getVersionMinor:      function()               { return _ofxChoc_ofBindings_getVersionMinor() },
    getVersionPatch:      function()               { return _ofxChoc_ofBindings_getVersionPatch() },
    getTimestampString:   function(fmt="")         { return _ofxChoc_ofBindings_getTimestampString(fmt) },
    map:  function(v,i0,i1,o0,o1,clamp=false)    { return _ofxChoc_ofBindings_map(v,i0,i1,o0,o1,clamp) },
    clamp: function(v,min,max)                    { return _ofxChoc_ofBindings_clamp(v,min,max) },
    lerp:  function(a,b,t)                        { return _ofxChoc_ofBindings_lerp(a,b,t) },
    norm:  function(v,min,max)                    { return _ofxChoc_ofBindings_norm(v,min,max) },
};
)");
    } catch(choc::javascript::Error &e) {
        ofLogError() << e.what();
    }
}
