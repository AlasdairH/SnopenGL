#version 430

in vec2 txcoord;

out vec4 out_Colour;

void main(void)
{
	vec2 uv = txcoord;
	uv.y *= -1.0;
	//vec3 t = texture(tex0, uv).rgb;
	vec3 t = vec3(1.0f, 1.0f, 1.0f);
	out_Colour = vec4(t, 1.0f);
}