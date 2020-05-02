// Attributes
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aUV;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;

void main()
{
	gl_Position = vec4(aPosition.x, aPosition.y, 0.0, 1.0);
	vUV = aUV;
}