#pragma once
#include "ofMain.h"
#include "registry.h"
#include "helpers.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfSerialBindings(choc::javascript::Context& ctx, ObjectStore<ofSerial>& store)
{
    ctx.registerFunction("_ofxChoc_serial_create", [&store](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createInt32(store.add(std::make_shared<ofSerial>()));
    });
    // setup(portName, baud) or setup(portIndex, baud)
    ctx.registerFunction("_ofxChoc_serial_setup", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            int baud = args[2] ? (int)numVal(args[2]) : 9600;
            if (args[1] && args[1]->getType().isString())
                return choc::value::createBool(o->setup(std::string(args[1]->getString()), baud));
            else
                return choc::value::createBool(o->setup(args[1] ? (int)numVal(args[1]) : 0, baud));
        }
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_serial_close", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->close();
        return {};
    });
    ctx.registerFunction("_ofxChoc_serial_isInitialized", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isInitialized());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_serial_available", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createInt32(o->available());
        return choc::value::createInt32(0);
    });
    ctx.registerFunction("_ofxChoc_serial_readByte", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createInt32(o->readByte());
        return choc::value::createInt32(-1);
    });
    ctx.registerFunction("_ofxChoc_serial_writeByte", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->writeByte(static_cast<unsigned char>(args[1] ? (int)numVal(args[1]) : 0));
        return {};
    });
    // Read up to maxBytes into a JS string (for ASCII protocols)
    ctx.registerFunction("_ofxChoc_serial_readString", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            int maxBytes = args[1] ? (int)numVal(args[1]) : 256;
            int avail    = o->available();
            int toRead   = std::min(avail, maxBytes);
            if (toRead <= 0) return choc::value::createString("");
            std::string result(toRead, '\0');
            o->readBytes(reinterpret_cast<unsigned char*>(&result[0]), (size_t)toRead);
            return choc::value::createString(result);
        }
        return choc::value::createString("");
    });
    ctx.registerFunction("_ofxChoc_serial_writeString", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            std::string s = args[1] ? std::string(args[1]->getString()) : "";
            o->writeBytes(reinterpret_cast<const unsigned char*>(s.data()), s.size());
        }
        return {};
    });
    ctx.registerFunction("_ofxChoc_serial_flush", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->flush();
        return {};
    });
    ctx.registerFunction("_ofxChoc_serial_listDevices", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            auto devs = o->getDeviceList();
            auto arr  = choc::value::createEmptyArray();
            for (auto& d : devs) {
                auto obj = choc::value::createObject("");
                obj.addMember("id",   choc::value::createInt32(d.getDeviceID()));
                obj.addMember("path", choc::value::createString(d.getDevicePath()));
                obj.addMember("name", choc::value::createString(d.getDeviceName()));
                arr.addArrayElement(obj);
            }
            return arr;
        }
        return choc::value::createEmptyArray();
    });
    ctx.registerFunction("_ofxChoc_serial_destroy", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args[0]) store.remove((int)numVal(args[0]));
        return {};
    });

    try { ctx.evaluateExpression(R"(
var Serial = class {
    constructor()                  { this._id = _ofxChoc_serial_create(); }
    setup(portOrName, baud=9600)   { return _ofxChoc_serial_setup(this._id, portOrName, baud); }
    close()                        { _ofxChoc_serial_close(this._id); return this; }
    isInitialized()                { return _ofxChoc_serial_isInitialized(this._id); }
    available()                    { return _ofxChoc_serial_available(this._id); }
    readByte()                     { return _ofxChoc_serial_readByte(this._id); }
    writeByte(b)                   { _ofxChoc_serial_writeByte(this._id, b); return this; }
    readString(max=256)            { return _ofxChoc_serial_readString(this._id, max); }
    writeString(s)                 { _ofxChoc_serial_writeString(this._id, s); return this; }
    flush()                        { _ofxChoc_serial_flush(this._id); return this; }
    listDevices()                  { return _ofxChoc_serial_listDevices(this._id); }
    destroy()                      { _ofxChoc_serial_destroy(this._id); }
};
)"); } catch (choc::javascript::Error& e) { ofLogError("ofxChoc") << "Serial: " << e.what(); }
}
