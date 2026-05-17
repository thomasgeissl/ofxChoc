#pragma once

#include "ofMain.h"
#include "helpers.h"

inline void registerOfGlBindings(choc::javascript::Context &context)
{
    bindFn(context, "_ofxChoc_ofBindings_disableArbTex",        &ofDisableArbTex);
    bindFn(context, "_ofxChoc_ofBindings_enableArbTex",         &ofEnableArbTex);
    bindFn(context, "_ofxChoc_ofBindings_enableAlphaBlending",  &ofEnableAlphaBlending);
    bindFn(context, "_ofxChoc_ofBindings_disableAlphaBlending", &ofDisableAlphaBlending);
    bindFn(context, "_ofxChoc_ofBindings_enableDepthTest",      &ofEnableDepthTest);
    bindFn(context, "_ofxChoc_ofBindings_disableDepthTest",     &ofDisableDepthTest);
    bindFn(context, "_ofxChoc_ofBindings_pushMatrix",           &ofPushMatrix);
    bindFn(context, "_ofxChoc_ofBindings_popMatrix",            &ofPopMatrix);
    bindFn(context, "_ofxChoc_ofBindings_pushStyle",            &ofPushStyle);
    bindFn(context, "_ofxChoc_ofBindings_popStyle",             &ofPopStyle);
    bindFn(context, "_ofxChoc_ofBindings_loadIdentityMatrix",   &ofLoadIdentityMatrix);

    // ofTranslate: (x,y) or (x,y,z)
    context.registerFunction("_ofxChoc_ofBindings_translate",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            if (args.size() >= 3)
                ofTranslate(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2]));
            else
                ofTranslate(fromJS<float>(args[0]), fromJS<float>(args[1]));
            return {};
        });

    // ofScale: (x,y) or (x,y,z) or (s)
    context.registerFunction("_ofxChoc_ofBindings_scale",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            if (args.size() >= 3)
                ofScale(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2]));
            else if (args.size() == 2)
                ofScale(fromJS<float>(args[0]), fromJS<float>(args[1]));
            else
                ofScale(fromJS<float>(args[0]));
            return {};
        });

    // ofRotateDeg: (deg) or (deg, x, y, z)
    context.registerFunction("_ofxChoc_ofBindings_rotateDeg",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            if (args.size() >= 4)
                ofRotateDeg(fromJS<float>(args[0]), fromJS<float>(args[1]),
                            fromJS<float>(args[2]), fromJS<float>(args[3]));
            else
                ofRotateDeg(fromJS<float>(args[0]));
            return {};
        });

    // ofRotateXDeg / ofRotateYDeg / ofRotateZDeg
    context.registerFunction("_ofxChoc_ofBindings_rotateXDeg",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        { ofRotateXDeg(fromJS<float>(args[0])); return {}; });
    context.registerFunction("_ofxChoc_ofBindings_rotateYDeg",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        { ofRotateYDeg(fromJS<float>(args[0])); return {}; });
    context.registerFunction("_ofxChoc_ofBindings_rotateZDeg",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        { ofRotateZDeg(fromJS<float>(args[0])); return {}; });

    context.run(R"(
ofxChoc_gl = {
    disableArbTex:        function()            { return _ofxChoc_ofBindings_disableArbTex() },
    enableArbTex:         function()            { return _ofxChoc_ofBindings_enableArbTex() },
    enableAlphaBlending:  function()            { return _ofxChoc_ofBindings_enableAlphaBlending() },
    disableAlphaBlending: function()            { return _ofxChoc_ofBindings_disableAlphaBlending() },
    enableDepthTest:      function()            { return _ofxChoc_ofBindings_enableDepthTest() },
    disableDepthTest:     function()            { return _ofxChoc_ofBindings_disableDepthTest() },
    pushMatrix:           function()            { return _ofxChoc_ofBindings_pushMatrix() },
    popMatrix:            function()            { return _ofxChoc_ofBindings_popMatrix() },
    pushStyle:            function()            { return _ofxChoc_ofBindings_pushStyle() },
    popStyle:             function()            { return _ofxChoc_ofBindings_popStyle() },
    loadIdentityMatrix:   function()            { return _ofxChoc_ofBindings_loadIdentityMatrix() },
    translate:            function(x,y,z=0)     { return _ofxChoc_ofBindings_translate(x,y,z) },
    scale:                function(x,y,z=1)     { return _ofxChoc_ofBindings_scale(x,y,z) },
    rotateDeg:            function(d,x=0,y=0,z=1) { return _ofxChoc_ofBindings_rotateDeg(d,x,y,z) },
    rotateXDeg:           function(d)           { return _ofxChoc_ofBindings_rotateXDeg(d) },
    rotateYDeg:           function(d)           { return _ofxChoc_ofBindings_rotateYDeg(d) },
    rotateZDeg:           function(d)           { return _ofxChoc_ofBindings_rotateZDeg(d) },
};
)");
}
