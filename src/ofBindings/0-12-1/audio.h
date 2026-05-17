#pragma once
#include "ofMain.h"
#include "registry.h"
#include "helpers.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"
#include <mutex>
#include <cmath>
#include <algorithm>

// ── SoundStreamWrapper ────────────────────────────────────────────────────────
// Thread-safe bridge between the ofSoundStream audio callbacks (audio thread)
// and the JS main thread.
//
// Input:  audioIn fills a "back" buffer (no lock); getInputSamples() swaps it
//         to "front" under a mutex and returns a copy to JS.
// Output: JS pushes samples into a pre-allocated ring buffer (main thread);
//         audioOut drains the ring buffer (audio thread).
//         A snapshot of the last played buffer is kept for waveform viz.

struct SoundStreamWrapper : public ofBaseSoundOutput, public ofBaseSoundInput
{
    ofSoundStream stream;
    int sampleRate        = 44100;
    int bufferSize        = 512;
    int numInputChannels  = 0;
    int numOutputChannels = 0;

    // ── input side (audio thread writes, main thread reads) ───────────────────
    std::mutex         inputMutex;
    std::vector<float> inputFront;   // main thread reads (copy under lock)
    std::vector<float> inputBack;    // audio thread fills  (no lock needed)
    float              inputRMS = 0.f;

    // ── output side (main thread writes, audio thread reads) ──────────────────
    static constexpr size_t RING_CAP = 32768;  // ~0.37 s at 44100×2ch
    std::mutex         outputMutex;
    std::vector<float> outputRing;
    size_t             ringReadIdx  = 0;
    size_t             ringWriteIdx = 0;
    size_t             ringCount    = 0;
    std::vector<float> outputFront; // snapshot of last audioOut buffer (viz)
    float              outputRMS = 0.f;

    SoundStreamWrapper()
    {
        outputRing.assign(RING_CAP, 0.f);
    }

    ~SoundStreamWrapper()
    {
        stream.stop();
        stream.close();
    }

    bool setupInput(int sr, int bufSz, int numCh)
    {
        sampleRate      = sr;
        bufferSize      = bufSz;
        numInputChannels = numCh;
        inputFront.assign(bufSz * numCh, 0.f);
        inputBack.assign (bufSz * numCh, 0.f);

        ofSoundStreamSettings s;
        auto devs = stream.getMatchingDevices("default");
        if (!devs.empty()) s.setInDevice(devs[0]);
        s.setInListener(this);
        s.sampleRate         = sr;
        s.numInputChannels   = numCh;
        s.numOutputChannels  = 0;
        s.bufferSize         = bufSz;
        return stream.setup(s);
    }

    bool setupOutput(int sr, int bufSz, int numCh)
    {
        sampleRate       = sr;
        bufferSize       = bufSz;
        numOutputChannels = numCh;
        outputFront.assign(bufSz * numCh, 0.f);

        ofSoundStreamSettings s;
        auto devs = stream.getMatchingDevices("default");
        if (!devs.empty()) s.setOutDevice(devs[0]);
        s.setOutListener(this);
        s.sampleRate        = sr;
        s.numOutputChannels = numCh;
        s.numInputChannels  = 0;
        s.bufferSize        = bufSz;
        return stream.setup(s);
    }

    // ── audio thread callbacks ────────────────────────────────────────────────

    void audioIn(ofSoundBuffer& input) override
    {
        size_t n = std::min(input.getNumFrames() * input.getNumChannels(),
                            inputBack.size());
        float rms = 0.f;
        for (size_t i = 0; i < n; i++) {
            inputBack[i] = input[i];
            rms += input[i] * input[i];
        }
        rms = (n > 0) ? std::sqrt(rms / (float)n) : 0.f;

        std::lock_guard<std::mutex> lk(inputMutex);
        std::swap(inputFront, inputBack);
        inputRMS = rms;
    }

    void audioOut(ofSoundBuffer& output) override
    {
        size_t n = output.getNumFrames() * output.getNumChannels();

        std::lock_guard<std::mutex> lk(outputMutex);
        size_t got = std::min(n, ringCount);
        for (size_t i = 0; i < got; i++) {
            output[i] = outputRing[ringReadIdx];
            ringReadIdx = (ringReadIdx + 1) % RING_CAP;
        }
        for (size_t i = got; i < n; i++) output[i] = 0.f; // underrun → silence
        ringCount -= got;

        // snapshot for waveform visualization
        size_t snapN = std::min(n, outputFront.size());
        float  rms   = 0.f;
        for (size_t i = 0; i < snapN; i++) {
            outputFront[i] = output[i];
            rms += output[i] * output[i];
        }
        outputRMS = (snapN > 0) ? std::sqrt(rms / (float)snapN) : 0.f;
    }

    // ── main-thread helpers ───────────────────────────────────────────────────

    void pushOneSample(float s)
    {
        std::lock_guard<std::mutex> lk(outputMutex);
        if (ringCount < RING_CAP) {
            outputRing[ringWriteIdx] = s;
            ringWriteIdx = (ringWriteIdx + 1) % RING_CAP;
            ++ringCount;
        }
    }

    std::vector<float> snapshotInput()
    {
        std::lock_guard<std::mutex> lk(inputMutex);
        return inputFront;
    }

    std::vector<float> snapshotOutput()
    {
        std::lock_guard<std::mutex> lk(outputMutex);
        return outputFront;
    }

    float getInputRMS()
    {
        std::lock_guard<std::mutex> lk(inputMutex);
        return inputRMS;
    }

    float getOutputRMS()
    {
        std::lock_guard<std::mutex> lk(outputMutex);
        return outputRMS;
    }
};

// ── registerOfAudioBindings ───────────────────────────────────────────────────

inline void registerOfAudioBindings(choc::javascript::Context&    ctx,
                                     ObjectStore<ofSoundPlayer>&   players,
                                     ObjectStore<SoundStreamWrapper>& streams)
{
    // ── SoundPlayer ───────────────────────────────────────────────────────────

    ctx.registerFunction("_ofxChoc_sound_create", [&players](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createInt32(players.add(std::make_shared<ofSoundPlayer>()));
    });
    ctx.registerFunction("_ofxChoc_sound_load", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->load(args[1] ? std::string(args[1]->getString()) : "",
                                                    args[2] && args[2]->getBool()));
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_sound_unload", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1)) o->unload();
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_play", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1)) o->play();
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_stop", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1)) o->stop();
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_setVolume", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setVolume(args[1] ? (float)numVal(args[1]) : 1.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_setPan", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setPan(args[1] ? (float)numVal(args[1]) : 0.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_setSpeed", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setSpeed(args[1] ? (float)numVal(args[1]) : 1.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_setLoop", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setLoop(args[1] && args[1]->getBool());
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_setMultiPlay", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setMultiPlay(args[1] && args[1]->getBool());
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_isLoaded", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isLoaded());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_sound_isPlaying", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isPlaying());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_sound_getPosition", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getPosition());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_sound_setPosition", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = players.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setPosition(args[1] ? (float)numVal(args[1]) : 0.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_sound_destroy", [&players](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args[0]) players.remove((int)numVal(args[0]));
        return {};
    });

    // ── SoundStream ───────────────────────────────────────────────────────────

    ctx.registerFunction("_ofxChoc_stream_create", [&streams](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createInt32(streams.add(std::make_shared<SoundStreamWrapper>()));
    });
    ctx.registerFunction("_ofxChoc_stream_setupInput", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->setupInput(
                args[1] ? (int)numVal(args[1]) : 44100,
                args[2] ? (int)numVal(args[2]) : 512,
                args[3] ? (int)numVal(args[3]) : 1));
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_stream_setupOutput", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->setupOutput(
                args[1] ? (int)numVal(args[1]) : 44100,
                args[2] ? (int)numVal(args[2]) : 512,
                args[3] ? (int)numVal(args[3]) : 2));
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_stream_start", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1)) o->stream.start();
        return {};
    });
    ctx.registerFunction("_ofxChoc_stream_stop", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1)) o->stream.stop();
        return {};
    });
    ctx.registerFunction("_ofxChoc_stream_getSampleRate", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createInt32(o->sampleRate);
        return choc::value::createInt32(44100);
    });
    ctx.registerFunction("_ofxChoc_stream_getBufferSize", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createInt32(o->bufferSize);
        return choc::value::createInt32(512);
    });
    ctx.registerFunction("_ofxChoc_stream_getNumInputChannels", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createInt32(o->numInputChannels);
        return choc::value::createInt32(0);
    });
    ctx.registerFunction("_ofxChoc_stream_getNumOutputChannels", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createInt32(o->numOutputChannels);
        return choc::value::createInt32(0);
    });
    // Push one output sample into the ring buffer (called in a JS loop)
    ctx.registerFunction("_ofxChoc_stream_pushSample", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1))
            if (args[1]) o->pushOneSample((float)numVal(args[1]));
        return {};
    });
    // Return a JS array containing the latest captured input buffer
    ctx.registerFunction("_ofxChoc_stream_getInputSamples", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1)) {
            auto snap = o->snapshotInput();
            auto arr  = choc::value::createEmptyArray();
            for (float s : snap) arr.addArrayElement(choc::value::createFloat64(s));
            return arr;
        }
        return choc::value::createEmptyArray();
    });
    ctx.registerFunction("_ofxChoc_stream_getInputRMS", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getInputRMS());
        return choc::value::createFloat64(0);
    });
    // Return a JS array containing the last played output buffer (for waveform viz)
    ctx.registerFunction("_ofxChoc_stream_getLastOutputSamples", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1)) {
            auto snap = o->snapshotOutput();
            auto arr  = choc::value::createEmptyArray();
            for (float s : snap) arr.addArrayElement(choc::value::createFloat64(s));
            return arr;
        }
        return choc::value::createEmptyArray();
    });
    ctx.registerFunction("_ofxChoc_stream_getLastOutputRMS", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = streams.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createFloat64(o->getOutputRMS());
        return choc::value::createFloat64(0);
    });
    ctx.registerFunction("_ofxChoc_stream_destroy", [&streams](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args[0]) streams.remove((int)numVal(args[0]));
        return {};
    });

    // ── JS class definitions ──────────────────────────────────────────────────

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

var SoundStream = class {
    constructor()               { this._id = _ofxChoc_stream_create(); }
    setupInput(sr, buf, ch)     { return _ofxChoc_stream_setupInput(this._id, sr, buf, ch); }
    setupOutput(sr, buf, ch)    { return _ofxChoc_stream_setupOutput(this._id, sr, buf, ch); }
    start()                     { _ofxChoc_stream_start(this._id); return this; }
    stop()                      { _ofxChoc_stream_stop(this._id); return this; }
    getSampleRate()             { return _ofxChoc_stream_getSampleRate(this._id); }
    getBufferSize()             { return _ofxChoc_stream_getBufferSize(this._id); }
    getNumInputChannels()       { return _ofxChoc_stream_getNumInputChannels(this._id); }
    getNumOutputChannels()      { return _ofxChoc_stream_getNumOutputChannels(this._id); }
    getInputSamples()           { return _ofxChoc_stream_getInputSamples(this._id); }
    getInputRMS()               { return _ofxChoc_stream_getInputRMS(this._id); }
    getLastOutputSamples()      { return _ofxChoc_stream_getLastOutputSamples(this._id); }
    getLastOutputRMS()          { return _ofxChoc_stream_getLastOutputRMS(this._id); }
    // pushOutputSamples(arr): push a JS array of floats into the output ring
    // buffer one sample at a time.  The ring holds ~0.37 s of audio at 44100×2ch.
    pushOutputSamples(arr)      {
        for (var i = 0; i < arr.length; i++)
            _ofxChoc_stream_pushSample(this._id, arr[i]);
        return this;
    }
    destroy()                   { _ofxChoc_stream_destroy(this._id); }
};
)"); } catch (choc::javascript::Error& e) { ofLogError("ofxChoc") << "Audio: " << e.what(); }
}
