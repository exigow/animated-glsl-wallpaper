#version 150 core

in vec2 fragCoord;
out vec4 fragColor;
uniform float iTime;
uniform sampler2D iChannel0;
uniform vec2 iResolution;

float map(vec3 p) {
    float displacement =
        sin(p.x * 0.72 + iTime * 0.23) *
        sin(p.y * 1.61 + iTime * 0.12) *
        sin(p.z * 3.73 + iTime * 0.09);
    return length(p) - 4.0 + displacement * .15;
}

vec3 calculate_normal(vec3 pos) {
    vec2 e = vec2(1.0, -1.0);
    return normalize(e.xyy * map(pos + e.xyy) + e.yyx * map(pos + e.yyx) + e.yxy * map(pos + e.yxy) + e.xxx * map(pos + e.xxx));
}

void main() {
    vec3 ro = vec3(cos(iTime * .025) * 2, sin(iTime * .025) * 1.5, -4.5);
    vec3 rd = vec3((fragCoord * 2 - 1) * vec2(iResolution.x / iResolution.y, 1), 1);
    float tmin = 1.0;
    float tmax = 4.0;
    for (int i = 0; i < 16 && tmin < tmax; i++) {
        vec3 pos = ro + tmin * rd;
        float h = map(pos);
        float e = 1.0 - min(abs(h) * i, 1);
        if (abs(h) < 0.1) {
            vec3 n = calculate_normal(pos);
            fragColor.rgb = texture(iChannel0, n.xy * .5 + .5).rgb * e;
        } else {
            fragColor.rgb = vec3(e);
        }
        tmin += h;
    }
}
