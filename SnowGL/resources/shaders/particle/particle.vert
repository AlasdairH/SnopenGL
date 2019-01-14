#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 orthographicMatrix;
};

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_velocity;
layout (location = 2) in float in_lifetime;



uniform float timeStep = 0.02f;

out vec3 out_position;
out vec3 out_velocity;
out float out_lifetime;

void main()
{
    out_velocity = in_velocity + vec3(0, -0.1f, 0);
    out_position = in_position + (out_velocity * timeStep);
    out_lifetime = in_lifetime - 1;
}