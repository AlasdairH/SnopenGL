#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 u_modelMatrix;

void main()
{
	mat4 MVP = projectionMatrix * viewMatrix * u_modelMatrix;

    gl_Position = MVP * vec4(position, 1.0);
}
