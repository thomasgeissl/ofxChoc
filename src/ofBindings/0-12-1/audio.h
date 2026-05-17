#pragma once
#include "ofMain.h"
#include "registry.h"
#include "helpers.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfAudioBindings(choc::javascript::Context& ctx, ObjectStore<ofSoundPlayer>& store)
{
    ctx.registerFunction("_ofxChoc_sound_create", [&store](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createInt32(store.add(std::make_shared<ofSoundPlayer>()));
    });
    ctx.registerFunction("_ofxChoc_sound_load", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->load(args[1] ? std::string(args[1]->getString()) : "",
                                                    args[2] && args[2]->getBool()));
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_sound_unload", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->unload();
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_play", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->play();
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_stop", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->stop();
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_setVolume", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setVolume(args[1] ? (float)numVal(args[1]) : 1.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_setPan", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setPan(args[1] ? (float)numVal(args[1]) : 0.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_setSpeed", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setSpeed(args[1] ? (float)numVal(args[1]) : 1.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_setLoop", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setLoop(args[1] && args[1]->getBool());
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_setMultiPlay", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setMultiPlay(args[1] && args[1]->getBool());
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_isLoaded", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isLoaded());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_sound_isPlaying", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isPlaying());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_sound_getPosition", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getPosition());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_sound_setPosition", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setPosition(args[1] ? (float)numVal(args[1]) : 0.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_destroy", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args[0]) store.remove((int)numVal(args[0]));
        return {};
    });

    try { ctx.evaluateExpression(R"(
var SoundPlayer = class {
    constructor()                { this._id = _ofxChoc_sound_create(); }
    load(path, stream=false)     { return _ofxChoc_sound_load(this._id, path, stream); }
    unload()                     { _ofxChoc_sound_unload(this._id); return this; }
    play()                       { _ofxChoc_sound_play(this._id); return this; }
    stop()                       { _ofxChoc_sound_stop(this._id); return this; }
    setVolume(v)                 { _ofxChoc_sound_setVolume(this._id, v); return this; }
    setPan(p)                    { _ofxChoc_sound_setPan(this._id, p); return this; }
    setSpeed(s)                  { _ofxChoc_sound_setSpeed(this._id, s); return this; }
    setLoop(l=true)              { _ofxChoc_sound_setLoop(this._id, l); return this; }
    setMultiPlay(m=true)         { _ofxChoc_sound_setMultiPlay(this._id, m); return this; }
    isLoaded()                   { return _ofxChoc_sound_isLoaded(this._id); }
    isPlaying()                  { return _ofxChoc_sound_isPlaying(this._id); }
    getPosition()                { return _ofxChoc_sound_getPosition(this._id); }
    setPosition(pct)             { _ofxChoc_sound_setPosition(this._id, pct); return this; }
    destroy()                    { _ofxChoc_sound_destroy(this._id); }
};
)"); } catch (choc::javascript::Error& e) { ofLogError("ofxChoc") << "SoundPlayer: " << e.what(); }
}
