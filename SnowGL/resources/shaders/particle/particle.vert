#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_velocity;
layout (location = 2) in float in_lifetime;

out vec3 out_position;
out vec3 out_velocity;
out float out_lifetime;

void main()
{
    out_position = in_position * 2;
    out_velocity = in_velocity * 2;
    out_lifetime = in_lifetime - 1;
}