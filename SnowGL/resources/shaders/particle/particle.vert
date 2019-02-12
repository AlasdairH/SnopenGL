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

uniform samplerBuffer geometry_tbo;
uniform int u_triangleCount;

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
		 return false;

	w0 = origin - v0;
	a = -dot(n, w0);
	b = dot(n, direction);
	if (abs(b) < 0.1)
		return false;

	r = a / b;
	if (r < 0.0 || r > 1.0)
		return false;

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
		return false;
	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0)
		return false;

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

	if(u_simTime >= in_startTime)
	{
		float age = u_simTime - in_startTime;
		// TODO: remove if through lessthan
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

	// intersection test
	vec3 v0, v1, v2;
	vec3 point;
	int i;
	for (i = 0; i < u_triangleCount; i++)
	{
		v0 = texelFetch(geometry_tbo, i * 3).xyz;
		v1 = texelFetch(geometry_tbo, i * 3 + 1).xyz;
		v2 = texelFetch(geometry_tbo, i * 3 + 2).xyz;
		if (intersect(in_position.xyz, in_position.xyz - out_position.xyz, v0, v1, v2, point))
		{
			vec3 n = normalize(cross(v1 - v0, v2 - v0));
			out_position = vec4(point + reflect_vector(out_position.xyz - point, n), 1.0);
			out_velocity = 0.8 * reflect_vector(out_velocity, n);
		}
	}

	mat4 MVP = projectionMatrix * viewMatrix * u_modelMatrix;
    gl_Position = MVP * vec4(out_position.x, out_position.y, out_position.z, 1.0);
}