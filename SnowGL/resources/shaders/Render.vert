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
	vec4 buffer_position;		// the position of the spatial partition
	// pre compute
	vec4 positionBL;			// the bottom left position of the spatial partition (used to offset for always positive values
	vec4 binSize;				// the position of the spatial partition
	// data
	int bin[];					// the array of bins
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
layout (location = 9) out float frag_snowPerc;

// max snow depth
uniform float u_maxSnowDepth = 0.2f;			// the max offset of a position for snow (the offset when u_maxSnowBinValue is reached)
uniform int u_maxSnowBinValue = 300;			// the max value a bin of snow can hold (the value will go higher but will be clamped on use)
uniform float u_snowAccumulationSpeed = 1.0f;
uniform float u_snowColourChangeSpeed = 3.0f;
uniform bool u_useSnow = true;

// model matrix
uniform mat4 u_modelMatrix;
uniform mat4 u_depthSpaceMatrix;

int toIndex(vec3 _pos)
{
	int index = -1;
	// get the position in partition space
	vec3 psPos = _pos - vec3(positionBL);

	if(psPos.x < 0 || psPos.x > dimensions.x)
	{
		if(psPos.y < 0 || psPos.y > dimensions.y)
		{
			if(psPos.z < 0 || psPos.z > dimensions.z)
			{
				return 99999;
			}
			return 99999;
		}
		return 99999;
	}

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
	frag_colour = vec4(0, 0, 0, 1);
	frag_snowPerc = 0.0f;

	vec4 pos = (u_modelMatrix * (vec4(position, 1.0f)));

	if(u_useSnow && normal.y > 0.0f)
	{
		// get the bin the position lies in
		int accumulationBinIndex = toIndex(pos.xyz);
		// get the current amount of snow in the bin
		int snowDepth = bin[accumulationBinIndex];
		// get the percentage of how full the bin is
		float percentageDepth = clamp(float(snowDepth) / float(u_maxSnowBinValue), 0.0f, 1.0f);
		// send the depth at the current bin over to the fragment shader
		frag_snowPerc = percentageDepth;
		// offset the position by the percentage depth
		pos += vec4(0, u_maxSnowDepth, 0, 0) * percentageDepth * u_snowAccumulationSpeed;
	}

	mat4 VP = projectionMatrix * viewMatrix;

	gl_Position = VP * pos;
}
