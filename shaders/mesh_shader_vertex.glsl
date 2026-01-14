#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTextures;

uniform mat4 perspective;
uniform mat4 view;

out vec3 normalsOut;
out vec3 texturesOut;

void main() {
    gl_Position = perspective * view * vec4(aPos, 1.0f);
    texturesOut = aTextures;
}