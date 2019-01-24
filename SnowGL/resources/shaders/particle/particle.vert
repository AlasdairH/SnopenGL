#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 orthographicMatrix;
};

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec3 in_velocity;
layout (location = 2) in float in_startTime;
layout (location = 3) in float in_lifetime;

// rendering
uniform mat4 u_modelMatrix;

// particle system
uniform vec3 u_baseColour = vec3(1.0f, 0.07f, 0.58f);

// timing
uniform float u_deltaTime = 1.0f;
uniform float u_simTime = 0.0f;

// transform feedback outputs
out vec4 out_position;
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
	particleColour = vec4(u_baseColour, 0.0f);

	out_position = in_position;
	out_velocity = in_velocity;
	out_startTime = in_startTime;
    out_lifetime = in_lifetime;

	if(u_simTime >= in_startTime)
	{
		float age = u_simTime - in_startTime;
		if(age > in_lifetime)
		{
			// particle is past it's lifetime
			out_position = vec4(in_position.x, 10, in_position.z, 1.0f);
			out_velocity = vec3(0.0f);
			out_startTime = u_simTime;
			particleColour = vec4(u_baseColour, 0.0f);
		}
		else
		{
			// particle is alive and well so update it
			out_velocity += vec3(0.0f, -0.01f, 0.0f);
			out_position = vec4(in_position.xyz + (out_velocity * u_deltaTime), 1.0f);

			float agePerc = age / in_lifetime;

			//particleColour = vec4(u_baseColour * agePerc, 1.0f);
			vec3 startColour = vec3(1.0f, 1.0f, 1.0f);
			vec3 endColour = vec3(1.0f, 0.54f, 0.0f);
			particleColour = vec4(mix(startColour, endColour, agePerc), 1 - agePerc);
		}
	}

	mat4 MVP = projectionMatrix * viewMatrix * u_modelMatrix;
    gl_Position = MVP * vec4(out_position.x, out_position.y, out_position.z, 1.0);
}