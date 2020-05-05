// Uniforms
uniform sampler2D uColorTexture;
uniform vec3 uColor;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV;

void main()
{
	oColor = texture(uColorTexture, vUV);
	oColor *= vec4(uColor, 1.0);
}