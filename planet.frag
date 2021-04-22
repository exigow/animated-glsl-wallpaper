#version 150 core

in vec2 fragCoord;
out vec4 fragColor;
uniform float iTime;
uniform sampler2D iChannel0;
uniform vec2 iResolution;

void main() {
    vec2 f = (fragCoord - 0.5) * 4.0;
    fragColor = vec4(0, 0, 0, 1);
    for (float i = 1.0; i > 0.0; i -= 0.01) {
        vec3 p = vec3(f, sqrt(1.0 - dot(f * i, f * i)));
        vec3 color = texture(iChannel0, 0.04 * iTime + p.xy * .1 / p.z).rgb;
        if (color.r > (1.0 - i) + 0.1) {
            fragColor.rgb = color;
        }
    }
}


