OF_GLSL_SHADER_HEADER

uniform sampler2DRect tex0;
in vec2 vTexCoord;
out vec4 fragColor;

void main() {
    vec4 c   = texture(tex0, vTexCoord);
    fragColor = vec4(1.0 - c.r, 1.0 - c.g, 1.0 - c.b, c.a);
}
