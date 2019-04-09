#version 430 core 
layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
};

layout(location = 0) in vec4 vposition;

uniform mat4 u_modelMatrix;

void main() 
{
	mat4 modelViewMatrix = viewMatrix * u_modelMatrix;
	gl_Position = modelViewMatrix * vec4(vposition.xyz, 1.0f);
};