#pragma once

#include "ofMain.h"
#include "helpers.h"

inline void registerOfGraphicsBindings(choc::javascript::Context &context)
{
    // ofBackground is overloaded — dispatch on arg count
    context.registerFunction("_ofxChoc_ofBindings_background",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            if (args.size() == 3)
                ofBackground(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2]));
            else if (args.size() == 4)
                ofBackground(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2]), fromJS<float>(args[3]));
            return {};
        });

    // ofDrawCircle is overloaded (2D / 3D) — dispatch on arg count
    context.registerFunction("_ofxChoc_ofBindings_drawCircle",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            if (args.size() == 3)
                ofDrawCircle(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2]));
            else if (args.size() == 4)
                ofDrawCircle(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2]), fromJS<float>(args[3]));
            return {};
        });

    // ofDrawCurve: 8-arg (2D) or 12-arg (3D)
    context.registerFunction("_ofxChoc_ofBindings_drawCurve",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            if (args.size() == 8)
                ofDrawCurve(fromJS<float>(args[0]), fromJS<float>(args[1]),
                            fromJS<float>(args[2]), fromJS<float>(args[3]),
                            fromJS<float>(args[4]), fromJS<float>(args[5]),
                            fromJS<float>(args[6]), fromJS<float>(args[7]));
            else if (args.size() == 12)
                ofDrawCurve(fromJS<float>(args[0]),  fromJS<float>(args[1]),  fromJS<float>(args[2]),
                            fromJS<float>(args[3]),  fromJS<float>(args[4]),  fromJS<float>(args[5]),
                            fromJS<float>(args[6]),  fromJS<float>(args[7]),  fromJS<float>(args[8]),
                            fromJS<float>(args[9]),  fromJS<float>(args[10]), fromJS<float>(args[11]));
            return {};
        });

    // ofSetColor is overloaded — dispatch on arg count
    context.registerFunction("_ofxChoc_ofBindings_setColor",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            if (args.size() == 1)
                ofSetColor(fromJS<int>(args[0]));
            else if (args.size() == 3)
                ofSetColor(fromJS<int>(args[0]), fromJS<int>(args[1]), fromJS<int>(args[2]));
            else if (args.size() == 4)
                ofSetColor(fromJS<int>(args[0]), fromJS<int>(args[1]), fromJS<int>(args[2]), fromJS<int>(args[3]));
            return {};
        });

    // ofClear: (r,g,b,a) or (brightness,a)
    context.registerFunction("_ofxChoc_ofBindings_clear",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            if (args.size() >= 4)
                ofClear(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2]), fromJS<float>(args[3]));
            else if (args.size() == 3)
                ofClear(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2]));
            else if (args.size() == 2)
                ofClear(fromJS<float>(args[0]), fromJS<float>(args[1]));
            else if (args.size() == 1)
                ofClear(fromJS<float>(args[0]));
            return {};
        });

    bindFn(context, "_ofxChoc_ofBindings_fill",   &ofFill);
    bindFn(context, "_ofxChoc_ofBindings_noFill",  &ofNoFill);
    bindFn(context, "_ofxChoc_ofBindings_setLineWidth", &ofSetLineWidth);

    // ofDrawRectangle is overloaded — use 2D variant
    context.registerFunction("_ofxChoc_ofBindings_drawRectangle",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            ofDrawRectangle(fromJS<float>(args[0]), fromJS<float>(args[1]),
                            fromJS<float>(args[2]), fromJS<float>(args[3]));
            return {};
        });

    // ofDrawLine
    context.registerFunction("_ofxChoc_ofBindings_drawLine",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            if (args.size() == 4)
                ofDrawLine(fromJS<float>(args[0]), fromJS<float>(args[1]),
                           fromJS<float>(args[2]), fromJS<float>(args[3]));
            else if (args.size() == 6)
                ofDrawLine(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2]),
                           fromJS<float>(args[3]), fromJS<float>(args[4]), fromJS<float>(args[5]));
            return {};
        });

    // ofDrawEllipse
    context.registerFunction("_ofxChoc_ofBindings_drawEllipse",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            ofDrawEllipse(fromJS<float>(args[0]), fromJS<float>(args[1]),
                          fromJS<float>(args[2]), fromJS<float>(args[3]));
            return {};
        });

    // ofDrawBox: (size) or (x,y,z,size) or (x,y,z,w,h,d)
    context.registerFunction("_ofxChoc_ofBindings_drawBox",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            if (args.size() == 1)
                ofDrawBox(fromJS<float>(args[0]));
            else if (args.size() == 4)
                ofDrawBox(fromJS<float>(args[0]), fromJS<float>(args[1]),
                          fromJS<float>(args[2]), fromJS<float>(args[3]));
            else if (args.size() == 6)
                ofDrawBox(fromJS<float>(args[0]), fromJS<float>(args[1]), fromJS<float>(args[2]),
                          fromJS<float>(args[3]), fromJS<float>(args[4]), fromJS<float>(args[5]));
            return {};
        });

    // ofDrawSphere
    context.registerFunction("_ofxChoc_ofBindings_drawSphere",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            if (args.size() == 1)
                ofDrawSphere(fromJS<float>(args[0]));
            else if (args.size() == 4)
                ofDrawSphere(fromJS<float>(args[0]), fromJS<float>(args[1]),
                             fromJS<float>(args[2]), fromJS<float>(args[3]));
            return {};
        });

    // ofDrawBitmapString (template — force string overload)
    context.registerFunction("_ofxChoc_ofBindings_drawBitmapString",
        [](choc::javascript::ArgumentList args) -> choc::value::Value
        {
            ofDrawBitmapString(fromJS<std::string>(args[0]),
                               fromJS<float>(args[1]), fromJS<float>(args[2]));
            return {};
        });

    context.run(R"(
ofxChoc_graphics = {
    background:        function(r,g,b,a=255)                         { return _ofxChoc_ofBindings_background(r,g,b,a) },
    setColor:          function(r,g,b,a=255)                         { return _ofxChoc_ofBindings_setColor(r,g,b,a) },
    clear:             function(r,g,b,a=0)                           { return _ofxChoc_ofBindings_clear(r,g,b,a) },
    fill:              function()                                     { return _ofxChoc_ofBindings_fill() },
    noFill:            function()                                     { return _ofxChoc_ofBindings_noFill() },
    setLineWidth:      function(w)                                    { return _ofxChoc_ofBindings_setLineWidth(w) },
    drawCircle:        function(x,y,r)                               { return _ofxChoc_ofBindings_drawCircle(x,y,r) },
    drawEllipse:       function(x,y,w,h)                             { return _ofxChoc_ofBindings_drawEllipse(x,y,w,h) },
    drawRectangle:     function(x,y,w,h)                             { return _ofxChoc_ofBindings_drawRectangle(x,y,w,h) },
    drawLine:          function(x1,y1,x2,y2)                        { return _ofxChoc_ofBindings_drawLine(x1,y1,x2,y2) },
    drawCurve:         function(x0,y0,x1,y1,x2,y2,x3,y3)           { return _ofxChoc_ofBindings_drawCurve(x0,y0,x1,y1,x2,y2,x3,y3) },
    drawBox:           function(x,y,z,size)                          { return _ofxChoc_ofBindings_drawBox(x,y,z,size) },
    drawSphere:        function(x,y,z,r)                             { return _ofxChoc_ofBindings_drawSphere(x,y,z,r) },
    drawBitmapString:  function(str,x,y)                             { return _ofxChoc_ofBindings_drawBitmapString(str,x,y) },
};
)");
}
