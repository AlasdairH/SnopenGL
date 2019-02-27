#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

// model inputs
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

// world space transform feedback
layout (location = 3) out vec4 out_worldSpacePosition;

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

// model matrix
uniform mat4 u_modelMatrix;
uniform mat4 u_depthSpaceMatrix;

// textures
uniform sampler2D u_depthMap;
layout(location = 20, binding = 20) uniform samplerBuffer u_accumulationBuffer;

int toIndex(ivec3 _pos)
{
	return int((_pos.z * u_domainWidth * u_domainHeight) + (_pos.y * u_domainWidth) + _pos.x);
}

vec3 indexTo3D(int _index)
{
	int z = int(_index / (u_domainWidth * u_domainHeight));
	_index -= (z * u_domainWidth * u_domainHeight);
	int y = int(_index / ceil(u_domainWidth));
	int x = int(_index % int(ceil(u_domainWidth)));
	return vec3(x, y, z);
}

void main()
{
	frag_texCoord = texCoord;
	frag_normal = transpose(inverse(mat3(u_modelMatrix))) * normal;
	frag_pos = vec3(u_modelMatrix * vec4(position, 1.0));
	frag_posDepthSpace = u_depthSpaceMatrix * vec4(frag_pos.xyz, 1.0);

	vec4 pos = (u_modelMatrix * (vec4(position, 1.0f)));

	int accumulationBinIndex = toIndex(ivec3(floor(pos.xyz) + (u_domainOffset * 2)));
	vec4 snowDepthRGBA = texelFetch(u_accumulationBuffer, accumulationBinIndex);
	int snowDepth = int(snowDepthRGBA.x);
	float percentageDepth = snowDepth / u_maxSnowDepth;

	//pos += vec4(0, 1, 0, 0) * percentageDepth;

	vec4 texel = texelFetch(u_accumulationBuffer, 0);

	out_worldSpacePosition = pos;
	mat4 VP = projectionMatrix * viewMatrix;

    //gl_Position = out_worldSpacePosition;
	//gl_Position = MVP * vec4(position, 1.0f);
	gl_Position = VP * pos;
	frag_colour = vec4(texel.xyz, 1.0f);
}
