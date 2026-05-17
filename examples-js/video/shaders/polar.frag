OF_GLSL_SHADER_HEADER

// Polar-warp shader: maps the flat slitscan tape onto a clock-face ring.
// When drawn inside shader.begin()/end() with tape.draw(...), OF automatically
// binds the tape FBO texture to tex0 — no custom uniforms needed for the texture.
//
// gl_FragCoord gives the actual screen pixel, which we convert to polar coords
// and use to look up the correct column/row from the tape.

uniform sampler2DRect tex0;
uniform vec2 resolution;   // screen size in pixels
uniform vec2 tapeSize;     // tape FBO size in pixels

out vec4 fragColor;

#define PI 3.14159265359

void main() {
    // Flip Y so up is up (OF window origin is top-left, gl_FragCoord is bottom-left)
    vec2 pos    = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);
    vec2 center = resolution * 0.5;
    vec2 delta  = pos - center;

    float halfSize = min(resolution.x, resolution.y) * 0.5;
    float radius   = length(delta) / halfSize;
    float angle    = atan(delta.y, delta.x);   // -PI to PI

    // Ring bounds (as fraction of halfSize)
    float innerR = 0.18;
    float outerR = 0.50;

    if (radius < innerR || radius > outerR) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // Rotate so that second 0 is at the top, matching the original example
    float offsetAngle = angle + radians(186.0);
    float normAngle   = mod(offsetAngle / (2.0 * PI) + 1.0, 1.0);
    float tapeX = normAngle * tapeSize.x;

    // Map radial depth to tape Y
    float t = (radius - innerR) / (outerR - innerR);
    float tapeY = t * tapeSize.y;

    fragColor = texture(tex0, vec2(tapeX, tapeY));
}
