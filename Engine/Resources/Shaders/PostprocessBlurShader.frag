// Uniforms
uniform sampler2D uColorTexture;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV[11];

void main()
{
	oColor = vec4(0.0);
	float weights[] = float[](0.0093, 0.028002, 0.065984, 0.121703, 0.175713, 0.198596, 0.175713, 0.121703, 0.065984, 0.028002, 0.0093);

	for (int i = 0; i < 11; i++)
		oColor += texture(uColorTexture, vUV[i]) * weights[i];
	
	oColor.a = 1.0;
}