#pragma once
#include "ofMain.h"
#include "registry.h"
#include "helpers.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfVideoBindings(choc::javascript::Context&        ctx,
                                     ObjectStore<ofVideoPlayer>&        players,
                                     ObjectStore<ofVideoGrabber>&       grabbers)
{
    // ── VideoPlayer ──────────────────────────────────────────────────────────
    ctx.registerFunction("_ofxChoc_videoplayer_create", [&players](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createInt32(players.add(std::make_shared<ofVideoPlayer>()));
    });
    ctx.registerFunction("_ofxChoc_videoplayer_load", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->load(args[1] ? std::string(args[1]->getString()) : ""));
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_videoplayer_play", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1)) o->play();
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_stop", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1)) o->stop();
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_setPaused", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setPaused(args[1] ? args[1]->getBool() : false);
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_update", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1)) o->update();
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_draw", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1)) {
            float x = args[1] ? (float)numVal(args[1]) : 0;
            float y = args[2] ? (float)numVal(args[2]) : 0;
            if (args.size() >= 5)
                o->draw(x, y, (float)numVal(args[3]), (float)numVal(args[4]));
            else
                o->draw(x, y);
        }
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_isFrameNew", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isFrameNew());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_videoplayer_isPlaying", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isPlaying());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_videoplayer_isLoaded", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isLoaded());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_videoplayer_getWidth", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getWidth());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_videoplayer_getHeight", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getHeight());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_videoplayer_getDuration", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getDuration());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_videoplayer_getPosition", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getPosition());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_videoplayer_setPosition", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setPosition(args[1] ? (float)numVal(args[1]) : 0.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_setVolume", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setVolume(args[1] ? (float)numVal(args[1]) : 1.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_setLoop", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setLoopState((args[1] && args[1]->getBool()) ? OF_LOOP_NORMAL : OF_LOOP_NONE);
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_close", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1)) o->close();
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_setSpeed", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setSpeed(args[1] ? (float)numVal(args[1]) : 1.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_getSpeed", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getSpeed());
        return choc::value::createFloat64(1);
    });
    ctx.registerFunction("_ofxChoc_videoplayer_getCurrentFrame", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createInt32(o->getCurrentFrame());
        return choc::value::createInt32(0);
    });
    ctx.registerFunction("_ofxChoc_videoplayer_getTotalNumFrames", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createInt32(o->getTotalNumFrames());
        return choc::value::createInt32(0);
    });
    ctx.registerFunction("_ofxChoc_videoplayer_getIsMovieDone", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->getIsMovieDone());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_videoplayer_previousFrame", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1)) o->previousFrame();
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_nextFrame", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1)) o->nextFrame();
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_firstFrame", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1)) o->firstFrame();
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_setFrame", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setFrame(args[1] ? (int)numVal(args[1]) : 0);
        return {};
    });
    ctx.registerFunction("_ofxChoc_videoplayer_getPixelColor", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1)) {
            auto& pix = o->getPixels();
            if (pix.isAllocated()) {
                auto c = pix.getColor(args[1] ? (int)numVal(args[1]) : 0,
                                      args[2] ? (int)numVal(args[2]) : 0);
                auto obj = choc::value::createObject("");
                obj.addMember("r", choc::value::createFloat64(c.r));
                obj.addMember("g", choc::value::createFloat64(c.g));
                obj.addMember("b", choc::value::createFloat64(c.b));
                return obj;
            }
        }
        auto obj = choc::value::createObject("");
        obj.addMember("r", choc::value::createFloat64(0));
        obj.addMember("g", choc::value::createFloat64(0));
        obj.addMember("b", choc::value::createFloat64(0));
        return obj;
    });
    ctx.registerFunction("_ofxChoc_videoplayer_destroy", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args[0]) players.remove((int)numVal(args[0]));
        return {};
    });

    // ── VideoGrabber ─────────────────────────────────────────────────────────
    ctx.registerFunction("_ofxChoc_videograbber_create", [&grabbers](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createInt32(grabbers.add(std::make_shared<ofVideoGrabber>()));
    });
    ctx.registerFunction("_ofxChoc_videograbber_setup", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->setup(args[1] ? (int)numVal(args[1]) : 640,
                                                     args[2] ? (int)numVal(args[2]) : 480));
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_videograbber_update", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1)) o->update();
        return {};
    });
    ctx.registerFunction("_ofxChoc_videograbber_draw", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1)) {
            float x = args[1] ? (float)numVal(args[1]) : 0;
            float y = args[2] ? (float)numVal(args[2]) : 0;
            if (args.size() >= 5)
                o->draw(x, y, (float)numVal(args[3]), (float)numVal(args[4]));
            else
                o->draw(x, y);
        }
        return {};
    });
    ctx.registerFunction("_ofxChoc_videograbber_isFrameNew", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isFrameNew());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_videograbber_getWidth", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getWidth());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_videograbber_getHeight", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getHeight());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_videograbber_setDeviceID", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setDeviceID(args[1] ? (int)numVal(args[1]) : 0);
        return {};
    });
    ctx.registerFunction("_ofxChoc_videograbber_setDesiredFrameRate", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setDesiredFrameRate(args[1] ? (int)numVal(args[1]) : 30);
        return {};
    });
    ctx.registerFunction("_ofxChoc_videograbber_videoSettings", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1)) o->videoSettings();
        return {};
    });
    ctx.registerFunction("_ofxChoc_videograbber_setVerbose", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setVerbose(args[1] ? args[1]->getBool() : true);
        return {};
    });
    ctx.registerFunction("_ofxChoc_videograbber_getPixelColor", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1)) {
            auto& pix = o->getPixels();
            if (pix.isAllocated()) {
                auto c = pix.getColor(args[1] ? (int)numVal(args[1]) : 0,
                                      args[2] ? (int)numVal(args[2]) : 0);
                auto obj = choc::value::createObject("");
                obj.addMember("r", choc::value::createFloat64(c.r));
                obj.addMember("g", choc::value::createFloat64(c.g));
                obj.addMember("b", choc::value::createFloat64(c.b));
                return obj;
            }
        }
        auto obj = choc::value::createObject("");
        obj.addMember("r", choc::value::createFloat64(0));
        obj.addMember("g", choc::value::createFloat64(0));
        obj.addMember("b", choc::value::createFloat64(0));
        return obj;
    });
    ctx.registerFunction("_ofxChoc_videograbber_listDevices", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1)) {
            auto devs = o->listDevices();
            auto arr  = choc::value::createEmptyArray();
            for (auto& d : devs) {
                auto obj = choc::value::createObject("");
                obj.addMember("id",   choc::value::createInt32(d.id));
                obj.addMember("name", choc::value::createString(d.deviceName));
                arr.addArrayElement(obj);
            }
            return arr;
        }
        return choc::value::createEmptyArray();
    });
    ctx.registerFunction("_ofxChoc_videograbber_close", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = grabbers.get(args[0] ? (int)numVal(args[0]) : -1)) o->close();
        return {};
    });
    ctx.registerFunction("_ofxChoc_videograbber_destroy", [&grabbers](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args[0]) grabbers.remove((int)numVal(args[0]));
        return {};
    });

    try { ctx.evaluateExpression(R"(
var VideoPlayer = class {
    constructor()              { this._id = _ofxChoc_videoplayer_create(); }
    load(path)                 { return _ofxChoc_videoplayer_load(this._id, path); }
    play()                     { _ofxChoc_videoplayer_play(this._id); return this; }
    stop()                     { _ofxChoc_videoplayer_stop(this._id); return this; }
    pause(p=true)              { _ofxChoc_videoplayer_setPaused(this._id, p); return this; }
    update()                   { _ofxChoc_videoplayer_update(this._id); return this; }
    draw(x, y, w=-1, h=-1)    {
        if (w < 0) _ofxChoc_videoplayer_draw(this._id, x, y);
        else       _ofxChoc_videoplayer_draw(this._id, x, y, w, h);
        return this;
    }
    isFrameNew()               { return _ofxChoc_videoplayer_isFrameNew(this._id); }
    isPlaying()                { return _ofxChoc_videoplayer_isPlaying(this._id); }
    isLoaded()                 { return _ofxChoc_videoplayer_isLoaded(this._id); }
    getIsMovieDone()           { return _ofxChoc_videoplayer_getIsMovieDone(this._id); }
    getWidth()                 { return _ofxChoc_videoplayer_getWidth(this._id); }
    getHeight()                { return _ofxChoc_videoplayer_getHeight(this._id); }
    getDuration()              { return _ofxChoc_videoplayer_getDuration(this._id); }
    getPosition()              { return _ofxChoc_videoplayer_getPosition(this._id); }
    setPosition(pct)           { _ofxChoc_videoplayer_setPosition(this._id, pct); return this; }
    setVolume(v)               { _ofxChoc_videoplayer_setVolume(this._id, v); return this; }
    setLoop(l=true)            { _ofxChoc_videoplayer_setLoop(this._id, l); return this; }
    setSpeed(s)                { _ofxChoc_videoplayer_setSpeed(this._id, s); return this; }
    getSpeed()                 { return _ofxChoc_videoplayer_getSpeed(this._id); }
    getCurrentFrame()          { return _ofxChoc_videoplayer_getCurrentFrame(this._id); }
    getTotalNumFrames()        { return _ofxChoc_videoplayer_getTotalNumFrames(this._id); }
    previousFrame()            { _ofxChoc_videoplayer_previousFrame(this._id); return this; }
    nextFrame()                { _ofxChoc_videoplayer_nextFrame(this._id); return this; }
    firstFrame()               { _ofxChoc_videoplayer_firstFrame(this._id); return this; }
    setFrame(n)                { _ofxChoc_videoplayer_setFrame(this._id, n); return this; }
    getPixelColor(x, y)        { return _ofxChoc_videoplayer_getPixelColor(this._id, x, y); }
    close()                    { _ofxChoc_videoplayer_close(this._id); return this; }
    destroy()                  { _ofxChoc_videoplayer_destroy(this._id); }
};

var VideoGrabber = class {
    constructor()              { this._id = _ofxChoc_videograbber_create(); }
    setup(w=640, h=480)        { return _ofxChoc_videograbber_setup(this._id, w, h); }
    update()                   { _ofxChoc_videograbber_update(this._id); return this; }
    draw(x, y, w=-1, h=-1)    {
        if (w < 0) _ofxChoc_videograbber_draw(this._id, x, y);
        else       _ofxChoc_videograbber_draw(this._id, x, y, w, h);
        return this;
    }
    isFrameNew()               { return _ofxChoc_videograbber_isFrameNew(this._id); }
    getWidth()                 { return _ofxChoc_videograbber_getWidth(this._id); }
    getHeight()                { return _ofxChoc_videograbber_getHeight(this._id); }
    setDeviceID(id)            { _ofxChoc_videograbber_setDeviceID(this._id, id); return this; }
    setDesiredFrameRate(fps)   { _ofxChoc_videograbber_setDesiredFrameRate(this._id, fps); return this; }
    videoSettings()            { _ofxChoc_videograbber_videoSettings(this._id); return this; }
    setVerbose(v=true)         { _ofxChoc_videograbber_setVerbose(this._id, v); return this; }
    getPixelColor(x, y)        { return _ofxChoc_videograbber_getPixelColor(this._id, x, y); }
    listDevices()              { return _ofxChoc_videograbber_listDevices(this._id); }
    close()                    { _ofxChoc_videograbber_close(this._id); return this; }
    destroy()                  { _ofxChoc_videograbber_destroy(this._id); }
};
)"); } catch (choc::javascript::Error& e) { ofLogError("ofxChoc") << "Video: " << e.what(); }
}
