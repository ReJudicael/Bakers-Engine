// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout (location = 4) in vec4 BoneIDs;
layout (location = 5) in vec4 Weights;

const int MAX_BONES = 100;

// Uniforms
uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uCam;
uniform mat4 gBones[MAX_BONES];

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;
out vec3 unprojectedPos;
out vec3 camPos;
out vec3 normal;
out vec3 tangent;
out mat4 projCam;
out vec4 posboneTT;
out int id;
out int id2;
out int id3;
out int id4;
out float W;

void main()
{

	/*mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
    	BoneTransform += gBones[BoneIDs[1]] * Weights[1];
    	BoneTransform += gBones[BoneIDs[2]] * Weights[2];
    	BoneTransform += gBones[BoneIDs[3]] * Weights[3];*/

	vec4 posIn = vec4(aPosition, 1.0f);
	vec4 pos = Weights[0] * (gBones[int(BoneIDs[0])] * posIn) +
			Weights[1] * (gBones[int(BoneIDs[1])] * posIn) +
			Weights[2] * (gBones[int(BoneIDs[2])] * posIn) +
			Weights[3] * (gBones[int(BoneIDs[3])] * posIn);
	id = int(BoneIDs.x);
	id2 = int(BoneIDs.y);
	id3 = int(BoneIDs[2]);
	id4 = int(BoneIDs[3]);
	W = Weights[0];
	posboneTT = pos;
	//vec4 pos = BoneTransform * vec4(aPosition, 1.0);
	pos =  uModel * vec4(pos.xyz, 1.0f);
	unprojectedPos = pos.xyz;

	camPos = vec3(uCam[3][0], uCam[3][1], uCam[3][2]);
	projCam = uProj * uCam; 
	gl_Position = uProj * uCam * pos;
	vUV = aUV;

	//vec4 boneNormal = BoneTransform * vec4(aNormal,0.0);

	//normal = (uModel * boneNormal).xyz;
	
}