#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 orthographicMatrix;
};

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec3 in_velocity;
layout (location = 2) in float in_delay;

// rendering
uniform mat4 u_modelMatrix;

// particle system
uniform float u_spawnRange = 5.0f;
uniform vec3 u_baseColour = vec3(1.0f, 0.07f, 0.58f);

// timing
uniform float u_deltaTime = 1.0f;
uniform float u_simTime = 0.0f;

// transform feedback outputs
out vec4 out_position;
out vec3 out_velocity;
out float out_delay;

// fragment shader inputs
out vec4 particleColour;

vec4 when_eq(vec4 x, vec4 y) 
{
	return 1.0 - abs(sign(x - y));
}

vec4 when_gt(vec4 x, vec4 y) 
{
	return max(sign(x - y), 0.0);
}

void main()
{
	particleColour = vec4(u_baseColour, 1.0f) * when_gt(vec4(u_simTime), vec4(in_delay));

    out_velocity = in_velocity + (vec3(0, -0.01f, 0) * when_gt(vec4(u_simTime), vec4(in_delay)).xyz);
    out_position = (in_position + (vec4(out_velocity, 0.0f) * u_deltaTime)) * in_position.w;
    out_delay = in_delay;

	if(out_position.y < 0)
	{
		out_position = vec4(out_position.x, 10, out_position.z, 1.0f);
		out_velocity = vec3(0, 0, 0);
		out_delay += u_simTime;
	}

	mat4 MVP = projectionMatrix * viewMatrix * u_modelMatrix;
    gl_Position = MVP * vec4(in_position.x, in_position.y, in_position.z, 1.0);
}