#pragma once
#include "ofMain.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

// Pure JS math/geometry classes — no C++ backing needed, these are value types.
inline void registerOfMathBindings(choc::javascript::Context& ctx)
{
    try { ctx.evaluateExpression(R"(

var Vec2 = class {
    constructor(x=0, y=0)      { this.x=x; this.y=y; }
    add(v)                     { return new Vec2(this.x+v.x, this.y+v.y); }
    sub(v)                     { return new Vec2(this.x-v.x, this.y-v.y); }
    mul(s)                     { return new Vec2(this.x*s, this.y*s); }
    div(s)                     { return new Vec2(this.x/s, this.y/s); }
    dot(v)                     { return this.x*v.x + this.y*v.y; }
    length()                   { return Math.sqrt(this.x*this.x + this.y*this.y); }
    lengthSquared()             { return this.x*this.x + this.y*this.y; }
    normalize()                { const l=this.length(); return l>0 ? this.mul(1/l) : new Vec2(); }
    distance(v)                { return this.sub(v).length(); }
    lerp(v, t)                 { return new Vec2(this.x+(v.x-this.x)*t, this.y+(v.y-this.y)*t); }
    rotate(deg)                {
        const r=deg*Math.PI/180, c=Math.cos(r), s=Math.sin(r);
        return new Vec2(this.x*c - this.y*s, this.x*s + this.y*c);
    }
    angle()                    { return Math.atan2(this.y, this.x) * 180 / Math.PI; }
    set(x, y)                  { this.x=x; this.y=y; return this; }
    clone()                    { return new Vec2(this.x, this.y); }
    toArray()                  { return [this.x, this.y]; }
    toString()                 { return 'Vec2('+this.x+', '+this.y+')'; }
    static fromAngle(deg, len=1) {
        const r=deg*Math.PI/180;
        return new Vec2(Math.cos(r)*len, Math.sin(r)*len);
    }
};

var Vec3 = class {
    constructor(x=0, y=0, z=0) { this.x=x; this.y=y; this.z=z; }
    add(v)                      { return new Vec3(this.x+v.x, this.y+v.y, this.z+v.z); }
    sub(v)                      { return new Vec3(this.x-v.x, this.y-v.y, this.z-v.z); }
    mul(s)                      { return new Vec3(this.x*s, this.y*s, this.z*s); }
    div(s)                      { return new Vec3(this.x/s, this.y/s, this.z/s); }
    dot(v)                      { return this.x*v.x + this.y*v.y + this.z*v.z; }
    cross(v)                    {
        return new Vec3(this.y*v.z - this.z*v.y,
                        this.z*v.x - this.x*v.z,
                        this.x*v.y - this.y*v.x);
    }
    length()                    { return Math.sqrt(this.x*this.x + this.y*this.y + this.z*this.z); }
    lengthSquared()              { return this.x*this.x + this.y*this.y + this.z*this.z; }
    normalize()                 { const l=this.length(); return l>0 ? this.mul(1/l) : new Vec3(); }
    distance(v)                 { return this.sub(v).length(); }
    lerp(v, t)                  {
        return new Vec3(this.x+(v.x-this.x)*t, this.y+(v.y-this.y)*t, this.z+(v.z-this.z)*t);
    }
    set(x, y, z)                { this.x=x; this.y=y; this.z=z; return this; }
    clone()                     { return new Vec3(this.x, this.y, this.z); }
    xy()                        { return new Vec2(this.x, this.y); }
    toArray()                   { return [this.x, this.y, this.z]; }
    toString()                  { return 'Vec3('+this.x+', '+this.y+', '+this.z+')'; }
};

var Color = class {
    constructor(r=255, g=255, b=255, a=255) { this.r=r; this.g=g; this.b=b; this.a=a; }
    lerp(c, t)     {
        return new Color(this.r+(c.r-this.r)*t, this.g+(c.g-this.g)*t,
                         this.b+(c.b-this.b)*t, this.a+(c.a-this.a)*t);
    }
    clamp()        {
        return new Color(Math.max(0,Math.min(255,this.r)), Math.max(0,Math.min(255,this.g)),
                         Math.max(0,Math.min(255,this.b)), Math.max(0,Math.min(255,this.a)));
    }
    toHex()        { return ((this.r&255)<<16)|((this.g&255)<<8)|(this.b&255); }
    toHexString()  { return '#'+this.toHex().toString(16).padStart(6,'0'); }
    withAlpha(a)   { return new Color(this.r, this.g, this.b, a); }
    clone()        { return new Color(this.r, this.g, this.b, this.a); }
    toString()     { return 'Color('+this.r+','+this.g+','+this.b+','+this.a+')'; }
    static fromHex(hex) { return new Color((hex>>16)&255,(hex>>8)&255,hex&255,255); }
    static fromHexString(s) {
        s = s.replace('#','');
        return Color.fromHex(parseInt(s,16));
    }
    static fromHSB(h, s, b, a=255) {
        // h: 0-360, s: 0-1, b: 0-1
        const i=Math.floor(h/60)%6, f=h/60-Math.floor(h/60);
        const p=b*(1-s), q=b*(1-f*s), t=b*(1-(1-f)*s);
        let r,g,bv;
        switch(i){
            case 0: r=b;g=t;bv=p;break; case 1: r=q;g=b;bv=p;break;
            case 2: r=p;g=b;bv=t;break; case 3: r=p;g=q;bv=b;break;
            case 4: r=t;g=p;bv=b;break; default: r=b;g=p;bv=q;
        }
        return new Color(Math.round(r*255),Math.round(g*255),Math.round(bv*255),a);
    }
};

var Rect = class {
    constructor(x=0, y=0, w=0, h=0) { this.x=x; this.y=y; this.width=w; this.height=h; }
    get right()    { return this.x + this.width; }
    get bottom()   { return this.y + this.height; }
    get center()   { return new Vec2(this.x + this.width*0.5, this.y + this.height*0.5); }
    contains(px, py) { return px>=this.x && px<=this.right && py>=this.y && py<=this.bottom; }
    containsPoint(v)  { return this.contains(v.x, v.y); }
    intersects(r)     {
        return !(r.x > this.right || r.right < this.x ||
                 r.y > this.bottom || r.bottom < this.y);
    }
    expand(dx, dy=dx) { return new Rect(this.x-dx, this.y-dy, this.width+2*dx, this.height+2*dy); }
    scale(s)          { return new Rect(this.x*s, this.y*s, this.width*s, this.height*s); }
    translate(dx, dy) { return new Rect(this.x+dx, this.y+dy, this.width, this.height); }
    clone()           { return new Rect(this.x, this.y, this.width, this.height); }
    toString()        { return 'Rect('+this.x+','+this.y+','+this.width+'x'+this.height+')'; }
    static fromCenter(cx, cy, w, h) { return new Rect(cx-w*0.5, cy-h*0.5, w, h); }
};

)"); } catch (choc::javascript::Error& e) { ofLogError("ofxChoc") << "Math classes: " << e.what(); }
}
