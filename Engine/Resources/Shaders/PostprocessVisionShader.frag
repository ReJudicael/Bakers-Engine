// Uniforms
uniform sampler2D uColorTexture;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV;

vec3 coloredVision(vec3 col)
{ 
    vec3 c_r = vec3(0.625, 0.375, 0.00); 
    vec3 c_g = vec3(0.700, 0.300, 0.00); 
    vec3 c_b = vec3(0.000, 0.300, 0.70);
    
	vec3 rgb = vec3( dot(col.rgb, c_r), dot(col.rgb, c_g), dot(col.rgb, c_b));
	return rgb; 
}

void main()
{
	oColor = texture(uColorTexture, vUV);
	vec3 rgb = coloredVision(vec3(oColor));
	oColor = vec4(rgb, 1.0);
}