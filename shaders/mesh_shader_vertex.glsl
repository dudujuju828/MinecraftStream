#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTextures;

uniform mat4 perspective;
uniform mat4 view;

// REVIEW: normalsOut is declared but never assigned a value. The fragment shader doesn't
// use it either. Remove it to keep the shader clean.
out vec3 normalsOut;
out vec3 texturesOut;

void main() {
    // REVIEW: no model matrix is used — chunks are positioned via vertex offsets in CPU
    // code (addFace). This works but means chunk positions are baked into vertex data.
    // A model matrix uniform would allow repositioning chunks without rebuilding meshes.
    gl_Position = perspective * view * vec4(aPos, 1.0f);
    texturesOut = aTextures;
}