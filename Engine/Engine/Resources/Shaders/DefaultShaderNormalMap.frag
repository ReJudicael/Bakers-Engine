#version 330 core

// Varyings
in vec2 vUV;

// Uniforms
uniform sampler2D uColorTexture;
uniform sampler2D uNormalMap;

// Shader outputs
out vec4 oColor;

void main()
{
    oColor = texture(uNormalMap, vUV);
}