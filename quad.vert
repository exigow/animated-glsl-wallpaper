#version 150 core

out vec2 fragCoord;
const vec2 pos[4] = vec2[4](vec2(-1, 1), vec2(-1), vec2(1), vec2(1, -1));

void main() {
    fragCoord = vec2(0.5, -0.5) * pos[gl_VertexID] + 0.5;
    gl_Position = vec4(pos[gl_VertexID], 0, 1);
}