#version 330 core

layout (location = 0) in vec4 vertex;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * vec4(vertex.xy, 1.f, 1.f);
    TexCoords = vertex.zw;
}
