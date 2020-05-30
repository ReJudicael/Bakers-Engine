// Uniforms
uniform sampler2D uColorTexture;
uniform vec2 uResolution;
uniform float uThresold;
uniform vec3 uLineColor;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV;

void main()
{
	oColor = texture(uColorTexture, vUV);

    float dist = 0.0;
    for (int y = -1; y <= 1; y++) 
	{
        for (int x = -1; x <= 1; x++) 
		{
            // distance() here will be in range 0.0 to pow(2, 1/3)
            // which is a distance between opposite cube vertices (0,0,0 and 1,1,1),
            // so let's normalize it by dividing it by pow(2, 1/3) ~= 1.2599
            dist += distance(
                oColor.xyz,
                texture(uColorTexture, vUV + (vec2(x, y) / uResolution.xy)).xyz
            ) / 1.2599;
        }
    }
    if (dist > uThresold)
	    oColor.xyz = mix(oColor.rgb, uLineColor, dist);
}