#pragma once
#include "ofMain.h"
#include "registry.h"
#include "fbo.h"
#include "image.h"
#include "shader.h"
#include "mesh.h"
#include "video.h"
#include "audio.h"
#include "font.h"
#include "camera.h"
#include "serial.h"
#include "mathBindings.h"

// ── Registry ─────────────────────────────────────────────────────────────────
// Holds all C++ objects created from JavaScript.
// Call clear() on hot-reload so stale pointers are released before the new
// script runs.

struct Registry {
    ObjectStore<ofFbo>              fbos;
    ObjectStore<ofImage>            images;
    ObjectStore<ofShader>           shaders;
    ObjectStore<ofMesh>             meshes;
    ObjectStore<ofVideoPlayer>      videoPlayers;
    ObjectStore<ofVideoGrabber>     videoGrabbers;
    ObjectStore<ofSoundPlayer>      soundPlayers;
    ObjectStore<SoundStreamWrapper> soundStreams;
    ObjectStore<ofTrueTypeFont>     fonts;
    ObjectStore<ofCamera>           cameras;
    ObjectStore<ofEasyCam>          easyCams;
    ObjectStore<ofSerial>           serials;

    void clear() {
        fbos.clear();
        images.clear();
        shaders.clear();
        meshes.clear();
        videoPlayers.clear();
        videoGrabbers.clear();
        soundPlayers.clear();
        soundStreams.clear();
        fonts.clear();
        cameras.clear();
        easyCams.clear();
        serials.clear();
    }
};

// ── Registration ─────────────────────────────────────────────────────────────
// Call once after registerOfBindings() so that `of` already exists.
// Each binding file registers its C++ functions and defines a JS class.
// At the end we attach all classes onto the global `of` object.

inline void registerOfObjectBindings(choc::javascript::Context& ctx, Registry& reg)
{
    registerOfFboBindings    (ctx, reg.fbos);
    registerOfImageBindings  (ctx, reg.images);
    registerOfShaderBindings (ctx, reg.shaders);
    registerOfMeshBindings   (ctx, reg.meshes);
    registerOfVideoBindings  (ctx, reg.videoPlayers, reg.videoGrabbers);
    registerOfAudioBindings  (ctx, reg.soundPlayers, reg.soundStreams);
    registerOfFontBindings   (ctx, reg.fonts);
    registerOfCameraBindings (ctx, reg.cameras, reg.easyCams);
    registerOfSerialBindings (ctx, reg.serials);
    registerOfMathBindings   (ctx);

    // Attach to the global `of` namespace so users write e.g. new of.Fbo()
    try {
        ctx.evaluateExpression(R"(
of.Fbo          = Fbo;
of.Image        = Image;
of.Shader       = Shader;
of.Mesh         = Mesh;
of.VideoPlayer  = VideoPlayer;
of.VideoGrabber = VideoGrabber;
of.SoundPlayer  = SoundPlayer;
of.SoundStream  = SoundStream;
of.Font         = Font;
of.Camera       = Camera;
of.EasyCam      = EasyCam;
of.Serial       = Serial;
of.Vec2         = Vec2;
of.Vec3         = Vec3;
of.Color        = Color;
of.Rect         = Rect;
)");
    } catch (choc::javascript::Error& e) {
        ofLogError("ofxChoc") << "objectBindings namespace: " << e.what();
    }
}
