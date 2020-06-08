
// Shader outputs
uniform vec4 cColor1;
uniform vec4 cColor2;

in vec2 vUV;
out vec4 oColor;

void main()
{
	oColor = vec4((cColor1 * (1 - vUV.y) + cColor2 * vUV.y).xyz, 1.0f);
}