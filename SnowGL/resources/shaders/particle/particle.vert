#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 orthographicMatrix;
};

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec3 in_velocity;
layout (location = 2) in float in_lifetime;

uniform mat4 u_modelMatrix;
uniform vec3 u_randomStartingPos;
uniform float u_timeStep = 1.0f;

out vec4 out_position;
out vec3 out_velocity;
out float out_lifetime;

void main()
{
    out_velocity = in_velocity + vec3(0, -0.01f, 0);
    out_position = (in_position + (vec4(out_velocity, 0.0f) * u_timeStep)) * in_position.w;
    out_lifetime = in_lifetime - (u_timeStep * 1000.0f);   
	
    if(out_position.y < 0.0f)
	{
		out_position = vec4(u_randomStartingPos, out_position.w);
        out_velocity = vec3(0, 0, 0);
	}

	mat4 MVP = projectionMatrix * viewMatrix * u_modelMatrix;
    gl_Position = MVP * vec4(in_position.x, in_position.y, in_position.z, 1.0);
}