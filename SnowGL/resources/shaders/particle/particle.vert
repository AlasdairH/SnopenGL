#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec4 in_startPosition;
layout (location = 2) in vec3 in_velocity;
layout (location = 3) in float in_startTime;
layout (location = 4) in float in_lifetime;

// rendering
uniform mat4 u_modelMatrix;

// particle system
uniform vec4 u_startColour = vec4(1.0f, 0.07f, 0.58f, 1.0f);
uniform vec4 u_endColour = vec4(1.0f, 0.07f, 0.58f, 1.0f);
uniform vec3 u_globalWind = vec3(0.00f, 0.0f, 0.0f);

// timing
uniform float u_deltaTime = 1.0f;
uniform float u_simTime = 0.0f;

// transform feedback outputs
out vec4 out_position;
out vec4 out_startPosition;
out vec3 out_velocity;
out float out_startTime;
out float out_lifetime;

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
	particleColour = vec4(u_startColour.xyz, 0.0f);

	out_position = in_position;
	out_startPosition = in_startPosition;
	out_velocity = in_velocity;
	out_startTime = in_startTime;
    out_lifetime = in_lifetime;

	if(u_simTime >= in_startTime)
	{
		float age = u_simTime - in_startTime;
		// remove if through lessthan
		if(out_position.y <= 0)
		{
			out_velocity = vec3(0.0f);
		}
		if(age > in_lifetime)
		{
			// particle is past it's lifetime
			out_position = out_startPosition;
			out_velocity = vec3(0.0f);
			out_startTime = u_simTime;
		}
		else
		{
			// particle is alive and well so update it
			out_velocity += vec3(0.0f, -0.98f, 0.0f);
			out_velocity += u_globalWind;

			out_position = vec4(in_position.xyz + (out_velocity * u_deltaTime * u_deltaTime), 1.0f);

			float agePerc = age / in_lifetime;

			particleColour = mix(u_startColour, u_endColour, agePerc);
		}
	}

	mat4 MVP = projectionMatrix * viewMatrix * u_modelMatrix;
    gl_Position = MVP * vec4(out_position.x, out_position.y, out_position.z, 1.0);
}