#version 150

uniform float time;
uniform vec2  resolution;
uniform vec2  mouse;       // normalised 0..1
uniform float speed;
uniform float complexity;

in  vec2 vTexCoord;
out vec4 fragColor;

// Classic plasma — overlapping sine waves in screen space
void main() {
    vec2 uv = vTexCoord;

    float t  = time * speed;
    float cx = uv.x + 0.5 * sin(t * 0.3);
    float cy = uv.y + 0.5 * cos(t * 0.2);

    float v  = sin(uv.x * complexity + t);
    v += sin(complexity * (uv.x * sin(t * 0.5) + uv.y * cos(t * 0.3)) + t);
    v += sin(complexity * distance(uv, mouse) + t);
    v += sin(complexity * (distance(vec2(cx, cy), uv)) + t);

    v = v * 0.25 + 0.5;   // remap to 0..1

    vec3 col = vec3(
        sin(v * 3.14159),
        sin(v * 3.14159 + 2.094),
        sin(v * 3.14159 + 4.189)
    );

    fragColor = vec4(col * 0.5 + 0.5, 1.0);
}
