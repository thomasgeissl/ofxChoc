#pragma once
#include "ofMain.h"
#include "registry.h"
#include "helpers.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

// ── Camera helpers ────────────────────────────────────────────────────────────

template<typename CamT>
static void registerCameraCommonBindings(choc::javascript::Context&  ctx,
                                          ObjectStore<CamT>&           store,
                                          const std::string&           prefix)
{
    auto p = [&](const std::string& n){ return prefix + n; };

    ctx.registerFunction(p("_create"), [&store](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createInt32(store.add(std::make_shared<CamT>()));
    });
    ctx.registerFunction(p("_begin"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->begin();
        return {};
    });
    ctx.registerFunction(p("_end"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->end();
        return {};
    });
    ctx.registerFunction(p("_setPosition"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setPosition(args[1] ? (float)numVal(args[1]) : 0,
                           args[2] ? (float)numVal(args[2]) : 0,
                           args[3] ? (float)numVal(args[3]) : 0);
        return {};
    });
    ctx.registerFunction(p("_getPosition"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            auto pos = o->getPosition();
            auto obj = choc::value::createObject("");
            obj.addMember("x", choc::value::createFloat64(pos.x));
            obj.addMember("y", choc::value::createFloat64(pos.y));
            obj.addMember("z", choc::value::createFloat64(pos.z));
            return obj;
        }
        return choc::value::createObject("");
    });
    ctx.registerFunction(p("_lookAt"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->lookAt(glm::vec3(args[1] ? (float)numVal(args[1]) : 0,
                                args[2] ? (float)numVal(args[2]) : 0,
                                args[3] ? (float)numVal(args[3]) : 0));
        return {};
    });
    ctx.registerFunction(p("_setFov"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setFov(args[1] ? (float)numVal(args[1]) : 60.f);
        return {};
    });
    ctx.registerFunction(p("_getFov"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getFov());
        return choc::value::createFloat64(60);
    });
    ctx.registerFunction(p("_setNearClip"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setNearClip(args[1] ? (float)numVal(args[1]) : 0.1f);
        return {};
    });
    ctx.registerFunction(p("_setFarClip"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setFarClip(args[1] ? (float)numVal(args[1]) : 1000.f);
        return {};
    });
    ctx.registerFunction(p("_tilt"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->tiltDeg(args[1] ? (float)numVal(args[1]) : 0);
        return {};
    });
    ctx.registerFunction(p("_pan"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->panDeg(args[1] ? (float)numVal(args[1]) : 0);
        return {};
    });
    ctx.registerFunction(p("_roll"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->rollDeg(args[1] ? (float)numVal(args[1]) : 0);
        return {};
    });
    ctx.registerFunction(p("_reset"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->resetTransform();
        return {};
    });
    ctx.registerFunction(p("_destroy"), [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args[0]) store.remove((int)numVal(args[0]));
        return {};
    });
}

inline void registerOfCameraBindings(choc::javascript::Context&  ctx,
                                      ObjectStore<ofCamera>&       cameras,
                                      ObjectStore<ofEasyCam>&      easyCams)
{
    registerCameraCommonBindings(ctx, cameras,  "_ofxChoc_camera");
    registerCameraCommonBindings(ctx, easyCams, "_ofxChoc_easycam");

    // EasyCam extras
    ctx.registerFunction("_ofxChoc_easycam_enableMouse", [&easyCams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = easyCams.get(args[0] ? (int)numVal(args[0]) : -1)) o->enableMouseInput();
        return {};
    });
    ctx.registerFunction("_ofxChoc_easycam_disableMouse", [&easyCams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = easyCams.get(args[0] ? (int)numVal(args[0]) : -1)) o->disableMouseInput();
        return {};
    });
    ctx.registerFunction("_ofxChoc_easycam_setDistance", [&easyCams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = easyCams.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setDistance(args[1] ? (float)numVal(args[1]) : 100.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_easycam_getDistance", [&easyCams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = easyCams.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getDistance());
        return choc::value::createFloat64(0);
    });

    // Shared JS class factory — returns the class body as a string
    auto commonMethods = [](const std::string& pfx) -> std::string {
        return
            "    begin()              { " + pfx + "_begin(this._id); return this; }\n"
            "    end()                { " + pfx + "_end(this._id); return this; }\n"
            "    setPosition(x,y,z)   { " + pfx + "_setPosition(this._id,x,y,z); return this; }\n"
            "    getPosition()        { return " + pfx + "_getPosition(this._id); }\n"
            "    lookAt(x,y,z)        { " + pfx + "_lookAt(this._id,x,y,z); return this; }\n"
            "    setFov(f)            { " + pfx + "_setFov(this._id,f); return this; }\n"
            "    getFov()             { return " + pfx + "_getFov(this._id); }\n"
            "    setNearClip(n)       { " + pfx + "_setNearClip(this._id,n); return this; }\n"
            "    setFarClip(f)        { " + pfx + "_setFarClip(this._id,f); return this; }\n"
            "    tilt(deg)            { " + pfx + "_tilt(this._id,deg); return this; }\n"
            "    pan(deg)             { " + pfx + "_pan(this._id,deg); return this; }\n"
            "    roll(deg)            { " + pfx + "_roll(this._id,deg); return this; }\n"
            "    reset()              { " + pfx + "_reset(this._id); return this; }\n"
            "    destroy()            { " + pfx + "_destroy(this._id); }\n";
    };

    std::string cameraJS =
        "var Camera = class {\n"
        "    constructor() { this._id = _ofxChoc_camera_create(); }\n"
        + commonMethods("_ofxChoc_camera") +
        "};\n";

    std::string easycamJS =
        "var EasyCam = class {\n"
        "    constructor() { this._id = _ofxChoc_easycam_create(); }\n"
        + commonMethods("_ofxChoc_easycam") +
        "    enableMouseInput()   { _ofxChoc_easycam_enableMouse(this._id); return this; }\n"
        "    disableMouseInput()  { _ofxChoc_easycam_disableMouse(this._id); return this; }\n"
        "    setDistance(d)       { _ofxChoc_easycam_setDistance(this._id,d); return this; }\n"
        "    getDistance()        { return _ofxChoc_easycam_getDistance(this._id); }\n"
        "};\n";

    try { ctx.evaluateExpression(cameraJS + easycamJS); }
    catch (choc::javascript::Error& e) { ofLogError("ofxChoc") << "Camera: " << e.what(); }
}
