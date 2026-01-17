#version 330 core

out vec4 fragColor;
in vec3 texturesOut;

uniform sampler2DArray array_sampler;

void main() {
    vec4 sample = texture(array_sampler,vec3(texturesOut));
    if (texturesOut.z == 0) sample.a *= 0.5f;

    fragColor = sample; 
}