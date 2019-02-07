#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

layout (location = 3) out vec4 out_worldSpacePosition;

out vec2 frag_texCoord;
out vec3 frag_vert;
out vec3 frag_normal;

uniform mat4 u_modelMatrix;

void main()
{
	frag_texCoord = texCoord;
	frag_vert = position;
	frag_normal = normal;

	mat4 MVP = projectionMatrix * viewMatrix * u_modelMatrix;
	//MVP = orthographicMatrix * viewMatrix * u_modelMatrix;
	out_worldSpacePosition = MVP * vec4(position.x, position.y, position.z, 1.0);
    gl_Position = out_worldSpacePosition;
}
