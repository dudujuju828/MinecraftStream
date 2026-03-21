#version 330 core

out vec4 fragColor;
in vec3 texturesOut;

uniform sampler2DArray array_sampler;

void main() {
    vec4 sample = texture(array_sampler,vec3(texturesOut));
    // REVIEW: texturesOut.z == 0 checks the texture layer index. Layer 0 is REDSTONE
    // in the block_texture_map. Making it semi-transparent (alpha 0.5) seems intentional
    // but only works if alpha blending is enabled — currently glEnable(GL_BLEND) is only
    // set for the crosshair pass, not for chunks. So this alpha change has no visible effect.
    if (texturesOut.z == 0) sample.a *= 0.5f;

    fragColor = sample;
}