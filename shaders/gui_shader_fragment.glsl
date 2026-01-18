#version 330 core

in vec3 textureOut;
out vec4 fragColor;

uniform sampler2DArray sampler;

void main() {
    vec4 sampledColor = texture(sampler, textureOut);
    fragColor = sampledColor;
}