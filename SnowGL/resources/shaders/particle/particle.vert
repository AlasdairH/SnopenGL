#version 430 core

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

// transform feedback inputs
layout (location = 0) in vec4 in_position;
layout (location = 1) in vec4 in_startPosition;
layout (location = 2) in vec3 in_velocity;
layout (location = 3) in float in_startTime;
layout (location = 4) in float in_lifetime;

// transform feedback outputs
layout (location = 5) out vec4 out_position;
layout (location = 6) out vec4 out_startPosition;
layout (location = 7) out vec3 out_velocity;
layout (location = 8) out float out_startTime;
layout (location = 9) out float out_lifetime;

layout (location = 10) uniform int u_triangleCount;
layout (location = 11) uniform samplerBuffer geometry_tbo;

// rendering
layout (location = 12) uniform mat4 u_modelMatrix;

// particle system
layout (location = 13) uniform vec4 u_startColour = vec4(1.0f, 0.07f, 0.58f, 1.0f);
layout (location = 14) uniform vec4 u_endColour = vec4(1.0f, 0.07f, 0.58f, 1.0f);
layout (location = 15) uniform vec3 u_globalWind = vec3(0.00f, 0.0f, 0.0f);
layout (location = 16) uniform vec3 u_initialVelocity = vec3(0, -50.0f, 0);
layout (location = 17) uniform float u_collisionMultiplier = 10.0f;

// timing
uniform float u_deltaTime = 1.0f;
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
	if (length(n) < 0.1)
	{
		return false;
	}

	w0 = origin - v0;
	a = -dot(n, w0);
	b = dot(n, direction);
	if (abs(b) < 0.1)
	{
		return false;
	}

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

	//particleColour = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	return true;
}

vec3 reflect_vector(vec3 v, vec3 n)
{
	return v - 2.0 * dot(v, n) * n;
}

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

	vec4 newPosition = out_position;

	if(u_simTime >= in_startTime)
	{
		float age = u_simTime - in_startTime;
		// TODO: remove if through lessthan
		if(age > in_lifetime)
		{
			// particle is past it's lifetime
			out_position = out_startPosition;
			out_velocity = u_initialVelocity;
			out_startTime = u_simTime;
		}
		else
		{
			// particle is alive and well so update it
			//out_velocity += vec3(0.0f, -0.38f, 0.0f);
			out_velocity += u_globalWind;

			out_position = vec4(in_position.xyz + (out_velocity * u_deltaTime * u_deltaTime), 1.0f);

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
					vec3 n = normalize(cross(v1 - v0, v2 - v0));
					out_position = in_position;
					out_velocity = vec3(0, 0, 0);
				}
			}
		}
	}

	mat4 MVP = projectionMatrix * viewMatrix * u_modelMatrix;
    gl_Position = MVP * vec4(out_position.x, out_position.y, out_position.z, 1.0);
}
