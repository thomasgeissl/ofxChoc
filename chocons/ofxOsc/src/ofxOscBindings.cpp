// ofxOsc chocon — OSC bindings for ofJsRuntime
//
// Exposes of.OscReceiver and of.OscSender to JavaScript.
//
// Build: see ../Makefile
// Entry points exported:
//   ofxChoc_registerAddon(ctx) — called once at startup to register bindings
//   ofxChoc_clearAddon()       — called on hot-reload to release all live objects

#include "ofxOsc.h"
#include "choc_javascript.h"  // interface only, no QuickJS compiled in
#include "memory/choc_Base64.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <sstream>

// ── Minimal ObjectStore (no registry.h dependency) ───────────────────────────

template<typename T>
struct AddonObjectStore {
    std::unordered_map<int, std::shared_ptr<T>> items;
    int nextId = 0;
    int add(std::shared_ptr<T> obj) { int id = nextId++; items[id] = std::move(obj); return id; }
    T* get(int id) { auto it = items.find(id); return it != items.end() ? it->second.get() : nullptr; }
    void remove(int id) { items.erase(id); }
    void clear() { items.clear(); }
};

static AddonObjectStore<ofxOscReceiver> receivers;
static AddonObjectStore<ofxOscSender>   senders;

// ── Helpers ───────────────────────────────────────────────────────────────────

static double numVal(const choc::value::Value* v, double def = 0.0) noexcept {
    if (!v) return def;
    if (v->isFloat64()) return v->getFloat64();
    if (v->isFloat32()) return (double)v->getFloat32();
    if (v->isInt32())   return (double)v->getInt32();
    if (v->isInt64())   return (double)v->getInt64();
    return def;
}

static int idArg(choc::javascript::ArgumentList args, size_t idx = 0) {
    return (int)numVal(args[idx]);
}

// Serialise an ofxOscMessage to a JSON string so JS can JSON.parse() it.
static std::string messageToJson(const ofxOscMessage& msg) {
    auto escapeStr = [](const std::string& s) {
        std::string out;
        out.reserve(s.size() + 2);
        out += '"';
        for (char c : s) {
            if      (c == '"')  out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else if (c == '\n') out += "\\n";
            else if (c == '\r') out += "\\r";
            else                out += c;
        }
        out += '"';
        return out;
    };

    std::ostringstream json;
    json << "{\"address\":" << escapeStr(msg.getAddress())
         << ",\"remoteHost\":" << escapeStr(msg.getRemoteHost())
         << ",\"remotePort\":" << msg.getRemotePort()
         << ",\"args\":[";

    for (size_t i = 0; i < msg.getNumArgs(); i++) {
        if (i > 0) json << ",";
        switch (msg.getArgType(i)) {
            case OFXOSC_TYPE_INT32:   json << msg.getArgAsInt32(i);   break;
            case OFXOSC_TYPE_INT64:   json << msg.getArgAsInt64(i);   break;
            case OFXOSC_TYPE_FLOAT:   json << msg.getArgAsFloat(i);   break;
            case OFXOSC_TYPE_DOUBLE:  json << msg.getArgAsDouble(i);  break;
            case OFXOSC_TYPE_STRING:
            case OFXOSC_TYPE_SYMBOL:  json << escapeStr(msg.getArgAsString(i)); break;
            case OFXOSC_TYPE_TRUE:    json << "true";  break;
            case OFXOSC_TYPE_FALSE:   json << "false"; break;
            case OFXOSC_TYPE_BLOB: {
                ofBuffer buff = msg.getArgAsBlob(i);
                std::string b64 = choc::base64::encodeToString(buff.getData(), buff.size());
                json << "{\"type\":\"blob\",\"size\":" << buff.size()
                     << ",\"base64\":" << escapeStr(b64) << "}";
                break;
            }
            default:                  json << "null";  break;
        }
    }
    json << "]}";
    return json.str();
}

// ── Entry points ──────────────────────────────────────────────────────────────

extern "C" void ofxChoc_registerAddon(choc::javascript::Context& ctx) {

    // ── OscReceiver ───────────────────────────────────────────────────────────

    ctx.registerFunction("_oscReceiver_create", [](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createFloat64(receivers.add(std::make_shared<ofxOscReceiver>()));
    });

    ctx.registerFunction("_oscReceiver_setup", [](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* r = receivers.get(idArg(args))) r->setup((int)numVal(args[1]));
        return {};
    });

    ctx.registerFunction("_oscReceiver_hasWaiting", [](choc::javascript::ArgumentList args) -> choc::value::Value {
        auto* r = receivers.get(idArg(args));
        return choc::value::createBool(r ? r->hasWaitingMessages() : false);
    });

    // Returns the next message as a JSON string, or an empty string if none.
    ctx.registerFunction("_oscReceiver_nextMessage", [](choc::javascript::ArgumentList args) -> choc::value::Value {
        auto* r = receivers.get(idArg(args));
        if (!r || !r->hasWaitingMessages()) return choc::value::createString("");
        ofxOscMessage msg;
        r->getNextMessage(msg);
        return choc::value::createString(messageToJson(msg));
    });

    ctx.registerFunction("_oscReceiver_destroy", [](choc::javascript::ArgumentList args) -> choc::value::Value {
        receivers.remove(idArg(args));
        return {};
    });

    // ── OscSender ─────────────────────────────────────────────────────────────

    ctx.registerFunction("_oscSender_create", [](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createFloat64(senders.add(std::make_shared<ofxOscSender>()));
    });

    ctx.registerFunction("_oscSender_setup", [](choc::javascript::ArgumentList args) -> choc::value::Value {
        auto* s = senders.get(idArg(args));
        if (s && args[1] && args[2])
            s->setup(std::string(args[1]->getString()), (int)numVal(args[2]));
        return {};
    });

    // Variadic: _oscSender_send(id, address, arg0, arg1, ...)
    // Each arg is sent as float (number), string, or bool based on its JS type.
    ctx.registerFunction("_oscSender_send", [](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args.size() < 2) return {};
        auto* s = senders.get(idArg(args));
        if (!s) return {};
        ofxOscMessage m;
        m.setAddress(std::string(args[1]->getString()));
        for (size_t i = 2; i < args.size(); i++) {
            const auto* v = args[i];
            if (!v) continue;
            if (v->isString())       m.addStringArg(std::string(v->getString()));
            else if (v->isBool())    m.addBoolArg(v->getBool());
            else                     m.addFloatArg((float)numVal(v));
        }
        s->sendMessage(m);
        return {};
    });

    // Send a binary blob from a file (e.g. PNG). Returns bytes sent, or 0 on failure.
    ctx.registerFunction("_oscSender_sendBlobFromFile", [](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args.size() < 3) return choc::value::createFloat64(0);
        auto* s = senders.get(idArg(args));
        if (!s || !args[1] || !args[2]) return choc::value::createFloat64(0);
        ofBuffer buff = ofBufferFromFile(std::string(args[2]->getString()), true);
        if (buff.size() == 0) return choc::value::createFloat64(0);
        ofxOscMessage m;
        m.setAddress(std::string(args[1]->getString()));
        m.addBlobArg(buff);
        s->sendMessage(m);
        return choc::value::createFloat64((double)buff.size());
    });

    // Send a binary blob from a base64-encoded string. Returns bytes sent, or 0 on failure.
    ctx.registerFunction("_oscSender_sendBlob", [](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args.size() < 3) return choc::value::createFloat64(0);
        auto* s = senders.get(idArg(args));
        if (!s || !args[1] || !args[2]) return choc::value::createFloat64(0);
        std::string bytes;
        if (!choc::base64::decodeToContainer(bytes, std::string_view(args[2]->getString())))
            return choc::value::createFloat64(0);
        ofBuffer buff(bytes.data(), bytes.size());
        ofxOscMessage m;
        m.setAddress(std::string(args[1]->getString()));
        m.addBlobArg(buff);
        s->sendMessage(m);
        return choc::value::createFloat64((double)buff.size());
    });

    ctx.registerFunction("_oscSender_destroy", [](choc::javascript::ArgumentList args) -> choc::value::Value {
        senders.remove(idArg(args));
        return {};
    });

    // ── JS class definitions ──────────────────────────────────────────────────
    // Classes live under ofx.osc.*  (ofx namespace is created if absent)

    ctx.evaluateExpression(R"(
if (typeof ofx === 'undefined') ofx = {};
if (typeof ofx.osc === 'undefined') ofx.osc = {};

ofx.osc.Receiver = class {
    constructor()        { this._id = _oscReceiver_create(); }
    setup(port)          { _oscReceiver_setup(this._id, port); }
    hasWaitingMessages() { return _oscReceiver_hasWaiting(this._id); }
    getNextMessage() {
        var s = _oscReceiver_nextMessage(this._id);
        return s ? JSON.parse(s) : null;
    }
    destroy()            { _oscReceiver_destroy(this._id); }
};

ofx.osc.Sender = class {
    constructor()                  { this._id = _oscSender_create(); }
    setup(host, port)              { _oscSender_setup(this._id, host, port); }
    send(address, ...args)         { _oscSender_send(this._id, address, ...args); }
    sendBlob(address, base64)      { return _oscSender_sendBlob(this._id, address, base64); }
    sendBlobFromFile(address, path){ return _oscSender_sendBlobFromFile(this._id, address, path); }
    destroy()                      { _oscSender_destroy(this._id); }
};

ofx.osc.isBlob = function(arg) {
    return arg && typeof arg === 'object' && arg.type === 'blob';
};
    )");
}

extern "C" void ofxChoc_clearChocon() {
    receivers.clear();
    senders.clear();
}
