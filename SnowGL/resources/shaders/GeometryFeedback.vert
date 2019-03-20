#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
};

// model inputs
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

// world space transform feedback
out vec4 out_worldSpacePosition;

// fragment shader outputs
out vec4 frag_colour;

uniform vec3 u_domainOffset = vec3(0);

// model matrix
uniform mat4 u_modelMatrix;

void main()
{
	vec4 pos = (u_modelMatrix * (vec4(position, 1.0f)));
	out_worldSpacePosition = pos;

	mat4 VP = projectionMatrix * viewMatrix;
	gl_Position = VP * pos;
}
