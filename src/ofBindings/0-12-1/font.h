#pragma once
#include "ofMain.h"
#include "registry.h"
#include "helpers.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfFontBindings(choc::javascript::Context& ctx, ObjectStore<ofTrueTypeFont>& store)
{
    ctx.registerFunction("_ofxChoc_font_create", [&store](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createInt32(store.add(std::make_shared<ofTrueTypeFont>()));
    });
    ctx.registerFunction("_ofxChoc_font_load", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            std::string path = args[1] ? std::string(args[1]->getString()) : "";
            int  size        = args[2] ? (int)numVal(args[2]) : 12;
            bool antiAlias   = args[3] ? args[3]->getBool() : true;
            bool fullCharSet = args[4] ? args[4]->getBool() : false;
            return choc::value::createBool(o->load(path, size, antiAlias, fullCharSet));
        }
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_font_drawString", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->drawString(args[1] ? std::string(args[1]->getString()) : "",
                          args[2] ? (float)numVal(args[2]) : 0,
                          args[3] ? (float)numVal(args[3]) : 0);
        return {};
    });
    ctx.registerFunction("_ofxChoc_font_drawStringAsShapes", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->drawStringAsShapes(args[1] ? std::string(args[1]->getString()) : "",
                                  args[2] ? (float)numVal(args[2]) : 0,
                                  args[3] ? (float)numVal(args[3]) : 0);
        return {};
    });
    ctx.registerFunction("_ofxChoc_font_getStringWidth", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->stringWidth(args[1] ? std::string(args[1]->getString()) : ""));
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_font_getStringHeight", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->stringHeight(args[1] ? std::string(args[1]->getString()) : ""));
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_font_getLineHeight", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getLineHeight());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_font_isLoaded", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isLoaded());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_font_destroy", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args[0]) store.remove((int)numVal(args[0]));
        return {};
    });

    try { ctx.evaluateExpression(R"(
var Font = class {
    constructor()                              { this._id = _ofxChoc_font_create(); }
    load(path, size=12, aa=true, full=false)   { return _ofxChoc_font_load(this._id, path, size, aa, full); }
    drawString(str, x, y)                      { _ofxChoc_font_drawString(this._id, str, x, y); return this; }
    drawStringAsShapes(str, x, y)              { _ofxChoc_font_drawStringAsShapes(this._id, str, x, y); return this; }
    getStringWidth(str)                        { return _ofxChoc_font_getStringWidth(this._id, str); }
    getStringHeight(str)                       { return _ofxChoc_font_getStringHeight(this._id, str); }
    getLineHeight()                            { return _ofxChoc_font_getLineHeight(this._id); }
    isLoaded()                                 { return _ofxChoc_font_isLoaded(this._id); }
    destroy()                                  { _ofxChoc_font_destroy(this._id); }
};
)"); } catch (choc::javascript::Error& e) { ofLogError("ofxChoc") << "Font: " << e.what(); }
}
