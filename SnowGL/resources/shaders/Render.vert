#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

// vec4 is used here for padding
layout (std430, binding = 1) buffer buffer_accumulation
{
	vec4 dimensions;			// the size of the spatial partition			
	vec4 resolution;			// the number of partitions in the width, height and depth
	vec4 buffer_position;				// the position of the spatial partition
	// pre compute
	vec4 positionBL;			// the bottom left position of the spatial partition (used to offset for always positive values
	vec4 binSize;				// the position of the spatial partition
	// data
	float bin[];				// the array of bins
};

// model inputs
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

// fragment shader outputs
layout (location = 4) out vec2 frag_texCoord;
layout (location = 5) out vec3 frag_normal;
layout (location = 6) out vec3 frag_pos;
layout (location = 7) out vec4 frag_posDepthSpace;
layout (location = 8) out vec4 frag_colour;

// domain
uniform float u_domainWidth = 7;
uniform float u_domainHeight = 7;
uniform float u_domainDepth = 3;
uniform vec3 u_domainOffset = vec3(0);

// max snow depth
uniform int u_maxSnowDepth = 1000;
uniform float u_snowAccumulationSpeed = 1.0f;
uniform bool u_useSnow = true;

// model matrix
uniform mat4 u_modelMatrix;
uniform mat4 u_depthSpaceMatrix;

// textures
layout(r32i, binding = 5) uniform iimageBuffer u_accumulationBuffer;

int toIndex(vec3 _pos)
{
	int index = -1;
	// get the position in partition space
	vec3 psPos = _pos - vec3(positionBL);
	ivec3 bin3d = ivec3(floor(psPos / vec3(binSize)));

	index = (bin3d.z * int(resolution.x) * int(resolution.y)) + (bin3d.y * int(resolution.x)) + bin3d.x;

	return index;
}

void main()
{
	frag_texCoord = texCoord;
	frag_normal = transpose(inverse(mat3(u_modelMatrix))) * normal;
	frag_pos = vec3(u_modelMatrix * vec4(position, 1.0));
	frag_posDepthSpace = u_depthSpaceMatrix * vec4(frag_pos.xyz, 1.0);

	vec4 pos = (u_modelMatrix * (vec4(position, 1.0f)));

	int accumulationBinIndex = toIndex(pos.xyz);
	vec4 snowDepthRGBA = imageLoad(u_accumulationBuffer, accumulationBinIndex);
	int snowDepth = int(snowDepthRGBA.x);
	float percentageDepth = float(snowDepth) / float(u_maxSnowDepth);

	if(u_useSnow)
	{
		pos += vec4(0, 1, 0, 0) * percentageDepth * u_snowAccumulationSpeed;
	}

	mat4 VP = projectionMatrix * viewMatrix;

    //gl_Position = out_worldSpacePosition;
	//gl_Position = MVP * vec4(position, 1.0f);
	gl_Position = VP * pos;
}
