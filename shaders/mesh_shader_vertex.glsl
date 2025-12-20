#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTextures;

uniform mat4 model;
uniform mat4 perspective;
uniform mat4 view;

out vec3 normalsOut;
out vec2 texturesOut;

void main() {
    gl_Position = perspective * view * model * vec4(aPos, 1.0f);
    normalsOut = aNormals;
    texturesOut = aTextures;
}