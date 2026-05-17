#pragma once
#include "ofMain.h"
#include "registry.h"
#include "helpers.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

static inline ofPrimitiveMode parseMeshMode(const std::string& s)
{
    if (s == "triangle_strip") return OF_PRIMITIVE_TRIANGLE_STRIP;
    if (s == "triangle_fan")   return OF_PRIMITIVE_TRIANGLE_FAN;
    if (s == "lines")          return OF_PRIMITIVE_LINES;
    if (s == "line_strip")     return OF_PRIMITIVE_LINE_STRIP;
    if (s == "line_loop")      return OF_PRIMITIVE_LINE_LOOP;
    if (s == "points")         return OF_PRIMITIVE_POINTS;
    return OF_PRIMITIVE_TRIANGLES;
}

inline void registerOfMeshBindings(choc::javascript::Context& ctx, ObjectStore<ofMesh>& store)
{
    ctx.registerFunction("_ofxChoc_mesh_create", [&store](choc::javascript::ArgumentList) -> choc::value::Value {
        return choc::value::createInt32(store.add(std::make_shared<ofMesh>()));
    });
    ctx.registerFunction("_ofxChoc_mesh_addVertex", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->addVertex(glm::vec3(args[1] ? (float)numVal(args[1]) : 0,
                                   args[2] ? (float)numVal(args[2]) : 0,
                                   args[3] ? (float)numVal(args[3]) : 0));
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_addColor", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->addColor(ofFloatColor(args[1] ? (float)numVal(args[1]) : 0,
                                     args[2] ? (float)numVal(args[2]) : 0,
                                     args[3] ? (float)numVal(args[3]) : 0,
                                     args[4] ? (float)numVal(args[4]) : 1));
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_addNormal", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->addNormal(glm::vec3(args[1] ? (float)numVal(args[1]) : 0,
                                   args[2] ? (float)numVal(args[2]) : 0,
                                   args[3] ? (float)numVal(args[3]) : 1));
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_addTexCoord", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->addTexCoord(glm::vec2(args[1] ? (float)numVal(args[1]) : 0,
                                     args[2] ? (float)numVal(args[2]) : 0));
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_addIndex", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->addIndex(args[1] ? (ofIndexType)numVal(args[1]) : 0);
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_draw", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->draw();
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_drawWireframe", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->drawWireframe();
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_drawVertices", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->drawVertices();
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_clear", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->clear();
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_setMode", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            o->setMode(parseMeshMode(args[1] ? std::string(args[1]->getString()) : "triangles"));
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_getNumVertices", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createInt32((int)o->getNumVertices());
        return choc::value::createInt32(0);
    });
    ctx.registerFunction("_ofxChoc_mesh_getNumIndices", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1))
            return choc::value::createInt32((int)o->getNumIndices());
        return choc::value::createInt32(0);
    });
    ctx.registerFunction("_ofxChoc_mesh_enableColors", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->enableColors();
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_disableColors", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->disableColors();
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_enableNormals", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->enableNormals();
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_disableNormals", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (auto* o = store.get(args[0] ? (int)numVal(args[0]) : -1)) o->disableNormals();
        return {};
    });
    ctx.registerFunction("_ofxChoc_mesh_destroy", [&store](choc::javascript::ArgumentList args) -> choc::value::Value {
        if (args[0]) store.remove((int)numVal(args[0]));
        return {};
    });

    try { ctx.evaluateExpression(R"(
var Mesh = class {
    constructor(mode='triangles')    {
        this._id = _ofxChoc_mesh_create();
        if (mode !== 'triangles') _ofxChoc_mesh_setMode(this._id, mode);
    }
    addVertex(x, y, z=0)            { _ofxChoc_mesh_addVertex(this._id, x, y, z); return this; }
    addColor(r, g, b, a=1)          { _ofxChoc_mesh_addColor(this._id, r, g, b, a); return this; }
    addNormal(x, y, z=1)            { _ofxChoc_mesh_addNormal(this._id, x, y, z); return this; }
    addTexCoord(u, v)               { _ofxChoc_mesh_addTexCoord(this._id, u, v); return this; }
    addIndex(i)                     { _ofxChoc_mesh_addIndex(this._id, i); return this; }
    draw()                          { _ofxChoc_mesh_draw(this._id); return this; }
    drawWireframe()                 { _ofxChoc_mesh_drawWireframe(this._id); return this; }
    drawVertices()                  { _ofxChoc_mesh_drawVertices(this._id); return this; }
    clear()                         { _ofxChoc_mesh_clear(this._id); return this; }
    setMode(mode)                   { _ofxChoc_mesh_setMode(this._id, mode); return this; }
    getNumVertices()                { return _ofxChoc_mesh_getNumVertices(this._id); }
    getNumIndices()                 { return _ofxChoc_mesh_getNumIndices(this._id); }
    enableColors()                  { _ofxChoc_mesh_enableColors(this._id); return this; }
    disableColors()                 { _ofxChoc_mesh_disableColors(this._id); return this; }
    enableNormals()                 { _ofxChoc_mesh_enableNormals(this._id); return this; }
    disableNormals()                { _ofxChoc_mesh_disableNormals(this._id); return this; }
    destroy()                       { _ofxChoc_mesh_destroy(this._id); }
};
)"); } catch (choc::javascript::Error& e) { ofLogError("ofxChoc") << "Mesh: " << e.what(); }
}
