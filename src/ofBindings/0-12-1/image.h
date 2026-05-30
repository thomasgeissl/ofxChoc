#pragma once
#include "ofMain.h"
#include "registry.h"
#include "helpers.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"
#include "../../libs/choc/memory/choc_Base64.h"

inline void registerOfImageBindings(choc::javascript::Context& ctx, ObjectStore<ofImage>& store)
{
    ctx.registerFunction("_ofxChoc_image_create", [&store](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createInt32(store.add(std::make_shared<ofImage>()));
    });
    ctx.registerFunction("_ofxChoc_image_load", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->load(args[1] ? std::string(args[1]->getString()) : ""));
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_image_loadFromBase64", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            if (!args[1]) return choc::value::createBool(false);
            std::string bytes;
            if (!choc::base64::decodeToContainer(bytes, std::string_view(args[1]->getString())))
                return choc::value::createBool(false);
            ofBuffer buff(bytes.data(), bytes.size());
            return choc::value::createBool(o->load(buff));
        }
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_image_save", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->save(args[1] ? std::string(args[1]->getString()) : "");
        return {};
    });
    ctx.registerFunction("_ofxChoc_image_allocate", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->allocate(args[1] ? (int)numVal(args[1]) : 0,
                        args[2] ? (int)numVal(args[2]) : 0,
                        OF_IMAGE_COLOR_ALPHA);
        return {};
    });
    ctx.registerFunction("_ofxChoc_image_draw", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
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
    ctx.registerFunction("_ofxChoc_image_resize", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->resize(args[1] ? (int)numVal(args[1]) : 0, args[2] ? (int)numVal(args[2]) : 0);
        return {};
    });
    ctx.registerFunction("_ofxChoc_image_grabScreen", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->grabScreen(args[1] ? (int)numVal(args[1]) : 0,
                          args[2] ? (int)numVal(args[2]) : 0,
                          args[3] ? (int)numVal(args[3]) : ofGetWidth(),
                          args[4] ? (int)numVal(args[4]) : ofGetHeight());
        return {};
    });
    ctx.registerFunction("_ofxChoc_image_update", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->update();
        return {};
    });
    ctx.registerFunction("_ofxChoc_image_getWidth", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getWidth());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_image_getHeight", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getHeight());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_image_getColor", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            auto c = o->getColor(args[1] ? (int)numVal(args[1]) : 0,
                                 args[2] ? (int)numVal(args[2]) : 0);
            auto obj = choc::value::createObject("");
            obj.addMember("r", choc::value::createFloat64(c.r));
            obj.addMember("g", choc::value::createFloat64(c.g));
            obj.addMember("b", choc::value::createFloat64(c.b));
            obj.addMember("a", choc::value::createFloat64(c.a));
            return obj;
        }
        return choc::value::createObject("");
    });
    ctx.registerFunction("_ofxChoc_image_setColor", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            int x = args[1] ? (int)numVal(args[1]) : 0;
            int y = args[2] ? (int)numVal(args[2]) : 0;
            o->setColor(x, y, ofColor(args[3] ? (int)numVal(args[3]) : 0,
                                      args[4] ? (int)numVal(args[4]) : 0,
                                      args[5] ? (int)numVal(args[5]) : 0,
                                      args[6] ? (int)numVal(args[6]) : 255));
        }
        return {};
    });
    ctx.registerFunction("_ofxChoc_image_isAllocated", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isAllocated());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_image_mirror", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->mirror(args[1] ? args[1]->getBool() : false, args[2] ? args[2]->getBool() : false);
        return {};
    });
    ctx.registerFunction("_ofxChoc_image_destroy", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args[0]) store.remove((int)numVal(args[0]));
        return {};
    });

    try { ctx.evaluateExpression(R"(
var Image = class {
    constructor()               { this._id = _ofxChoc_image_create(); }
    load(path)                  { return _ofxChoc_image_load(this._id, path); }
    loadFromBase64(base64)      { return _ofxChoc_image_loadFromBase64(this._id, base64); }
    save(path)                  { _ofxChoc_image_save(this._id, path); return this; }
    allocate(w, h)              { _ofxChoc_image_allocate(this._id, w, h); return this; }
    draw(x, y, w=-1, h=-1)     {
        if (w < 0) _ofxChoc_image_draw(this._id, x, y);
        else       _ofxChoc_image_draw(this._id, x, y, w, h);
        return this;
    }
    resize(w, h)                { _ofxChoc_image_resize(this._id, w, h); return this; }
    grabScreen(x, y, w, h)     { _ofxChoc_image_grabScreen(this._id, x, y, w, h); return this; }
    update()                    { _ofxChoc_image_update(this._id); return this; }
    getWidth()                  { return _ofxChoc_image_getWidth(this._id); }
    getHeight()                 { return _ofxChoc_image_getHeight(this._id); }
    getColor(x, y)              { return _ofxChoc_image_getColor(this._id, x, y); }
    setColor(x, y, r, g, b, a=255) { _ofxChoc_image_setColor(this._id, x, y, r, g, b, a); return this; }
    isAllocated()               { return _ofxChoc_image_isAllocated(this._id); }
    mirror(vertical, horizontal){ _ofxChoc_image_mirror(this._id, vertical, horizontal); return this; }
    destroy()                   { _ofxChoc_image_destroy(this._id); }
};
)"); } catch (choc::javascript::Error& e) { ofLogError("ofxChoc") << "Image: " << e.what(); }
}
