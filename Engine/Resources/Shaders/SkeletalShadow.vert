// Attributes
layout(location = 0) in vec3 aPosition;
layout (location = 4) in vec4 BoneIDs;
layout (location = 5) in vec4 Weights;

const int MAX_BONES = 100;

// Uniforms
uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uCam;
uniform mat4 uBones[MAX_BONES];

void main()
{
	vec4 posIn = vec4(aPosition, 1.0f);
	vec4 pos = Weights[0] * (uBones[int(BoneIDs[0])] * posIn) +
			Weights[1] * (uBones[int(BoneIDs[1])] * posIn) +
			Weights[2] * (uBones[int(BoneIDs[2])] * posIn) +
			Weights[3] * (uBones[int(BoneIDs[3])] * posIn);

	pos = uModel * vec4(pos.xyz, 1.0f);
	gl_Position = uProj * uCam * pos;
}