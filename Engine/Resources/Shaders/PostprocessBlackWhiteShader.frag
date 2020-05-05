// Uniforms
uniform sampler2D uColorTexture;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV;

void main()
{
	oColor = texture(uColorTexture, vUV);
	float medium = oColor.x + oColor.y + oColor.z;
	medium /= 3.0;
	oColor = vec4(medium, medium, medium, 1.0);
}