#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

uniform sampler2D u_depthMap;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

layout (location = 3) out vec4 out_worldSpacePosition;

layout (location = 4) out vec2 frag_texCoord;
layout (location = 5) out vec3 frag_normal;
layout (location = 6) out vec3 frag_pos;
layout (location = 7) out vec4 frag_posDepthSpace;

uniform mat4 u_modelMatrix;
uniform mat4 u_depthSpaceMatrix;

void main()
{
	frag_texCoord = texCoord;
	frag_normal = transpose(inverse(mat3(u_modelMatrix))) * normal;
	frag_pos = vec3(u_modelMatrix * vec4(position, 1.0));
	frag_posDepthSpace = u_depthSpaceMatrix * vec4(frag_pos.xyz, 1.0);

	mat4 MVP = projectionMatrix * viewMatrix * u_modelMatrix;
	//MVP = orthographicMatrix * viewMatrix * u_modelMatrix;
	out_worldSpacePosition = MVP * vec4(position, 1.0);


    gl_Position = out_worldSpacePosition;
}
