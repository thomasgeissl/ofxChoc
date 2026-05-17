#pragma once
#include "ofMain.h"
#include "registry.h"
#include "helpers.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfFboBindings(choc::javascript::Context& ctx, ObjectStore<ofFbo>& store)
{
    ctx.registerFunction("_ofxChoc_fbo_create", [&store](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createInt32(store.add(std::make_shared<ofFbo>()));
    });
    ctx.registerFunction("_ofxChoc_fbo_allocate", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            int  w   = args[1] ? (int)numVal(args[1]) : 0;
            int  h   = args[2] ? (int)numVal(args[2]) : 0;
            int  fmt = args[3] ? (int)numVal(args[3]) : GL_RGBA;
            int  smp = args[4] ? (int)numVal(args[4]) : 0;
            if (smp > 0) o->allocate(w, h, fmt, smp);
            else         o->allocate(w, h, fmt);
        }
        return {};
    });
    ctx.registerFunction("_ofxChoc_fbo_begin", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->begin();
        return {};
    });
    ctx.registerFunction("_ofxChoc_fbo_end", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->end();
        return {};
    });
    ctx.registerFunction("_ofxChoc_fbo_draw", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            float x = args[1] ? (float)numVal(args[1]) : 0;
            float y = args[2] ? (float)numVal(args[2]) : 0;
            if (args.size() >= 5)
                o->draw(x, y, (float)numVal(args[3]), (float)numVal(args[4]));
            else
                o->draw(x, y);
        }
        return {};
    });
    ctx.registerFunction("_ofxChoc_fbo_getWidth", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getWidth());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_fbo_getHeight", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getHeight());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_fbo_isAllocated", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isAllocated());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_fbo_drawSubsection", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            float x  = args[1] ? (float)numVal(args[1]) : 0;
            float y  = args[2] ? (float)numVal(args[2]) : 0;
            float w  = args[3] ? (float)numVal(args[3]) : 0;
            float h  = args[4] ? (float)numVal(args[4]) : 0;
            float sx = args[5] ? (float)numVal(args[5]) : 0;
            float sy = args[6] ? (float)numVal(args[6]) : 0;
            o->getTexture().drawSubsection(x, y, w, h, sx, sy);
        }
        return {};
    });
    ctx.registerFunction("_ofxChoc_fbo_destroy", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args[0]) store.remove((int)numVal(args[0]));
        return {};
    });

    try { ctx.evaluateExpression(R"(
var Fbo = class {
    constructor()                       { this._id = _ofxChoc_fbo_create(); }
    allocate(w, h, fmt=6408, samples=0) { _ofxChoc_fbo_allocate(this._id, w, h, fmt, samples); return this; }
    begin()                             { _ofxChoc_fbo_begin(this._id); return this; }
    end()                               { _ofxChoc_fbo_end(this._id); return this; }
    draw(x, y, w=-1, h=-1)             {
        if (w < 0) _ofxChoc_fbo_draw(this._id, x, y);
        else       _ofxChoc_fbo_draw(this._id, x, y, w, h);
        return this;
    }
    getWidth()    { return _ofxChoc_fbo_getWidth(this._id); }
    getHeight()   { return _ofxChoc_fbo_getHeight(this._id); }
    isAllocated() { return _ofxChoc_fbo_isAllocated(this._id); }
    drawSubsection(x, y, w, h, sx, sy) { _ofxChoc_fbo_drawSubsection(this._id, x, y, w, h, sx, sy); return this; }
    destroy()     { _ofxChoc_fbo_destroy(this._id); }
};
)"); } catch (choc::javascript::Error& e) { ofLogError("ofxChoc") << "Fbo: " << e.what(); }
}
