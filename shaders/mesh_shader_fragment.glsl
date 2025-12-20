#version 330 core

out vec4 fragColor;
in vec3 normalsOut;
in vec2 texturesOut;

void main() {
    fragColor = vec4(0.2f,0.2f,0.5f,1.0f);
}