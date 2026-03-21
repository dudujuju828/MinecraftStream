#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTex;

out vec3 textureOut;

void main() {
    // REVIEW: the 0.18f scale factor is hardcoded in the shader. If the crosshair size
    // needs to change, it requires editing and recompiling the shader. Pass it as a uniform
    // (or use the unused crossHairSize from crosshair.hpp).
    gl_Position = vec4(aPos*0.18f, 1.0f);
    textureOut = aTex;
}