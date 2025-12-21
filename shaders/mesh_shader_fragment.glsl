#version 330 core

out vec4 fragColor;
in vec3 normalsOut;
in vec2 texturesOut;

uniform sampler2D sampler;

void main() {
    fragColor = texture(sampler,texturesOut);
}