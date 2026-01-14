#version 330 core

out vec4 fragColor;
in vec3 texturesOut;

uniform sampler2DArray array_sampler;

void main() {
    fragColor = texture(array_sampler,vec3(texturesOut));
}