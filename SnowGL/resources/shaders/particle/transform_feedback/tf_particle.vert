#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
};

// vec4 is used here for padding
layout (std430, binding = 1) buffer buffer_accumulation
{
	vec4 dimensions;			// the size of the spatial partition			
	vec4 resolution;			// the number of partitions in the width, height and depth
	vec4 position;				// the position of the spatial partition
	// pre compute
	vec4 positionBL;			// the bottom left position of the spatial partition (used to offset for always positive values
	vec4 binSize;				// the position of the spatial partition
	// data
	int bin[];				// the array of bins
};

uniform int u_triangleCount;
layout(binding = 0) uniform samplerBuffer geometry_tbo;

// transform feedback inputs
layout (location = 0) in vec4 in_position;
in vec4 in_startPosition;
in vec3 in_velocity;
in float in_startTime;
in float in_lifetime;

// transform feedback outputs
out vec4 out_position;
out vec4 out_startPosition;
out vec3 out_velocity;
out float out_startTime;
out float out_lifetime;

// rendering
uniform mat4 u_modelMatrix;

// particle system
// colour
uniform vec4 u_startColour = vec4(1.0f, 0.07f, 0.58f, 1.0f);
uniform vec4 u_endColour = vec4(1.0f, 0.07f, 0.58f, 1.0f);
uniform vec4 u_collisionColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
// environment
uniform vec3 u_globalWind = vec3(0.0f, 0.0f, 0.0f);
// movement
uniform vec3 u_initialVelocity = vec3(0, -0.5f, 0);
uniform float u_collisionMultiplier = 1.0f;
// domain
uniform float u_domainWidth = 1;
uniform float u_domainHeight = 1;
uniform float u_domainDepth = 1;
uniform vec3 u_domainPosition = vec3(0, 0, 0);
uniform vec3 u_domainOffset = vec3(0, 0, 0);
uniform int u_accumulationSampleResolution = 10; // res x res x res

// timing
uniform float u_deltaTime = 0.016f;
uniform float u_simTime = 0.0f;

// fragment shader inputs
out vec4 particleColour;

bool intersect(vec3 origin, vec3 direction, vec3 v0, vec3 v1, vec3 v2, out vec3 point)
{
	vec3 u, v, n;
	vec3 w0, w;
	float r, a, b;

	u = (v1 - v0);
	v = (v2 - v0);
	n = cross(u, v);

	w0 = origin - v0;
	a = -dot(n, w0);
	b = dot(n, direction);

	r = a / b;
	if (r < 0.0 || r > 1.0)
	{
		return false;
	}

	point = origin + r * direction;

	float uu, uv, vv, wu, wv, D;

	uu = dot(u, u);
	uv = dot(u, v);
	vv = dot(v, v);
	w = point - v0;
	wu = dot(w, u);
	wv = dot(w, v);
	D = uv * uv - uu * vv;

	float s, t;

	s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0)
	{
		return false;
	}

	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0)
	{
		return false;
	}

	return true;
}

vec3 reflect_vector(vec3 v, vec3 n)
{
	return v - 2.0 * dot(v, n) * n;
}

int toIndex(vec3 _pos)
{
	int index = -1;
	// get the position in partition space
	vec3 psPos = _pos - vec3(positionBL);
	ivec3 bin3d = ivec3(floor(psPos / vec3(binSize)));

	index = (bin3d.z * int(resolution.x) * int(resolution.y)) + (bin3d.y * int(resolution.x)) + bin3d.x;

	return index;
}

void main()
{
	// buffer 0
	out_position = in_position;
	out_startPosition = in_startPosition;
	out_velocity = in_velocity;
	out_startTime = in_startTime;
    out_lifetime = in_lifetime;

	if(u_simTime >= in_startTime)
	{
		float age = u_simTime - in_startTime;
		if(age > in_lifetime)
		{
			// particle is past it's lifetime
			out_position = out_startPosition;
			out_velocity = u_initialVelocity;
			out_startTime = u_simTime;
		}
		// if the collision index of the particle isn't -1 (if it's stuck to something)
		else if(out_position.w >= 0.0f)
		{
			particleColour = u_collisionColour;
			out_position = vec4(0, -1000, 0, out_position.w);
		}
		// out of bounds check
		/*
		else if(out_position.x > u_domainWidth + u_domainPosition.x || out_position.x < -u_domainWidth + u_domainPosition.x
		|| out_position.y > u_domainDepth + u_domainPosition.y || out_position.y < -u_domainDepth + u_domainPosition.y
		|| out_position.z > u_domainWidth + u_domainPosition.z || out_position.z < -u_domainWidth + u_domainPosition.z
		)
		{
			particleColour = vec4(0.0, 1.0, 0.0, 0.0);
			out_position = vec4(0.0, 0.0, 0.0, 0.0);
		}
		*/
		else
		{
			// if we got here the particle is alive and well so update it
			out_velocity += (u_globalWind * u_deltaTime);
			out_position = vec4(in_position.xyz + (out_velocity * u_deltaTime), out_position.w);

			float agePerc = age / in_lifetime;
			particleColour = mix(u_startColour, u_endColour, agePerc);

			// ------------------------- intersection test -------------------------
			vec3 v0, v1, v2;
			vec3 point;
			int i;
			for (i = 0; i < u_triangleCount; i++)
			{
				v0 = texelFetch(geometry_tbo, i * 3).xyz;
				v1 = texelFetch(geometry_tbo, i * 3 + 1).xyz;
				v2 = texelFetch(geometry_tbo, i * 3 + 2).xyz;
				
				if (intersect(in_position.xyz, (in_position.xyz - out_position.xyz) * u_collisionMultiplier, v0, v1, v2, point))
				{
					//vec3 n = normalize(cross(v1 - v0, v2 - v0));
					out_position = vec4(point.xyz, i);
					out_velocity = vec3(0, 0, 0);
					int index = toIndex(out_position.xyz);
					atomicAdd(bin[index], 1);
				}
			}
		}
	}

	vec4 finalPosition = vec4(out_position.xyz, 1.0);
	//finalPosition -= positionBL;

	mat4 MVP = projectionMatrix * viewMatrix * u_modelMatrix;
    gl_Position = MVP * finalPosition;
}