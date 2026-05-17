#pragma once
#include "ofMain.h"
#include "registry.h"
#include "helpers.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfShaderBindings(choc::javascript::Context& ctx, ObjectStore<ofShader>& store)
{
    ctx.registerFunction("_ofxChoc_shader_create", [&store](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createInt32(store.add(std::make_shared<ofShader>()));
    });
    // load(basePath) or load(vertPath, fragPath [, geomPath])
    ctx.registerFunction("_ofxChoc_shader_load", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            std::string a = args[1] ? std::string(args[1]->getString()) : "";
            std::string b = args[2] ? std::string(args[2]->getString()) : "";
            std::string g = args[3] ? std::string(args[3]->getString()) : "";
            bool ok = b.empty() ? o->load(a) : o->load(a, b, g);
            return choc::value::createBool(ok);
        }
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_shader_begin", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->begin();
        return {};
    });
    ctx.registerFunction("_ofxChoc_shader_end", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->end();
        return {};
    });
    ctx.registerFunction("_ofxChoc_shader_isLoaded", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createBool(o->isLoaded());
        return choc::value::createBool(false);
    });
    ctx.registerFunction("_ofxChoc_shader_uniform1f", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setUniform1f(args[1] ? std::string(args[1]->getString()) : "",
                            args[2] ? (float)numVal(args[2]) : 0.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_shader_uniform1i", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setUniform1i(args[1] ? std::string(args[1]->getString()) : "",
                            args[2] ? (int)numVal(args[2]) : 0);
        return {};
    });
    ctx.registerFunction("_ofxChoc_shader_uniform2f", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setUniform2f(args[1] ? std::string(args[1]->getString()) : "",
                            args[2] ? (float)numVal(args[2]) : 0.f,
                            args[3] ? (float)numVal(args[3]) : 0.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_shader_uniform3f", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setUniform3f(args[1] ? std::string(args[1]->getString()) : "",
                            args[2] ? (float)numVal(args[2]) : 0.f,
                            args[3] ? (float)numVal(args[3]) : 0.f,
                            args[4] ? (float)numVal(args[4]) : 0.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_shader_uniform4f", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setUniform4f(args[1] ? std::string(args[1]->getString()) : "",
                            args[2] ? (float)numVal(args[2]) : 0.f,
                            args[3] ? (float)numVal(args[3]) : 0.f,
                            args[4] ? (float)numVal(args[4]) : 0.f,
                            args[5] ? (float)numVal(args[5]) : 0.f);
        return {};
    });
    ctx.registerFunction("_ofxChoc_shader_uniform2fv", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) {
            std::string name = args[1] ? std::string(args[1]->getString()) : "";
            float x = args[2] ? (float)numVal(args[2]) : 0.f;
            float y = args[3] ? (float)numVal(args[3]) : 0.f;
            o->setUniform2f(name, x, y);
        }
        return {};
    });
    ctx.registerFunction("_ofxChoc_shader_setUniformTexture", [](choc::javascript::ArgumentList) -> choc::value::Value {
        // stub — texture binding requires a texture registry; bind via fbo.getTexture() in a future update
        return {};
    });
    ctx.registerFunction("_ofxChoc_shader_destroy", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args[0]) store.remove((int)numVal(args[0]));
        return {};
    });

    try { ctx.evaluateExpression(R"(
var Shader = class {
    constructor()                      { this._id = _ofxChoc_shader_create(); }
    load(a, b, g)                      {
        return b ? _ofxChoc_shader_load(this._id, a, b, g||'')
                 : _ofxChoc_shader_load(this._id, a);
    }
    begin()                            { _ofxChoc_shader_begin(this._id); return this; }
    end()                              { _ofxChoc_shader_end(this._id); return this; }
    isLoaded()                         { return _ofxChoc_shader_isLoaded(this._id); }
    setUniform1f(name, v)              { _ofxChoc_shader_uniform1f(this._id, name, v); return this; }
    setUniform1i(name, v)              { _ofxChoc_shader_uniform1i(this._id, name, v); return this; }
    setUniform2f(name, x, y)          { _ofxChoc_shader_uniform2f(this._id, name, x, y); return this; }
    setUniform3f(name, x, y, z)       { _ofxChoc_shader_uniform3f(this._id, name, x, y, z); return this; }
    setUniform4f(name, x, y, z, w)    { _ofxChoc_shader_uniform4f(this._id, name, x, y, z, w); return this; }
    destroy()                          { _ofxChoc_shader_destroy(this._id); }
};
)"); } catch (choc::javascript::Error& e) { ofLogError("ofxChoc") << "Shader: " << e.what(); }
}
