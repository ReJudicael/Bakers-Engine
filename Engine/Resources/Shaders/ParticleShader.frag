// Uniforms
uniform sampler2D uColorTexture;
uniform vec4 uParticleColor;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV;

void main()
{
	oColor = texture(uColorTexture, vUV) * uParticleColor;
	if (oColor.a < 0.2)
		discard;
}