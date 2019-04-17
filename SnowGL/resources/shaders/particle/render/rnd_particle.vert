#version 430 core 
layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
};

layout (location = 0) in vec4	in_vposition;
layout (location = 3) in float	in_mass;

out float mass[1];

uniform mat4 u_modelMatrix;

void main() 
{
	mat4 modelViewMatrix = viewMatrix * u_modelMatrix;
	gl_Position = modelViewMatrix * vec4(in_vposition.xyz, 1.0f);
	gl_Position = vec4(gl_Position.xyz, in_mass);
	mass[0] = in_mass;
};