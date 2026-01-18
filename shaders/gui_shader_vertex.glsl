#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTex;

out vec3 textureOut;

void main() {

    gl_Position = vec4(aPos*0.18f, 1.0f);
    textureOut = aTex;
}