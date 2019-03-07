#version 430 core

// textures
layout (binding = 0) uniform sampler2D u_diffuseTexture;
uniform sampler2D u_depthMap;
uniform sampler2D u_snowTexture;

layout (location = 4) in vec2 frag_texCoord;
layout (location = 5) in vec3 frag_normal;
layout (location = 6) in vec3 frag_pos;
layout (location = 7) in vec4 frag_posDepthSpace;
layout (location = 8) in vec4 frag_colour;
layout (location = 9) in float frag_snowPerc;

uniform vec3 u_lightPos = vec3(0, 5, 0);
uniform vec3 u_lightColour = vec3(0.5f);

uniform bool u_useTexture = true;
uniform vec4 u_fragColour = vec4(1.0, 0.0, 0.0, 1.0);

out vec4 outputColour;

float calculateOcclusion(vec4 fragPosDepthSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosDepthSpace.xyz / fragPosDepthSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosDepth as coords)
    float closestDepth = texture(u_depthMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;

	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
	// colour pass
	float shadow = calculateOcclusion(frag_posDepthSpace);
	// 1 = not in shadow
	// 0 = in shadow

	vec3 colour;
	if(shadow == 1.0f)
	{
		colour = texture(u_diffuseTexture, frag_texCoord).xyz;
	}
	else
	{
		//colour = texture(u_snowTexture, frag_texCoord).xyz;
		colour = texture(u_diffuseTexture, frag_texCoord).xyz;
	}
	colour = mix(colour, vec3(1, 1, 1), frag_snowPerc);

	vec3 ambient = 0.40f * colour;

	// diffuse
	vec3 lightDir = normalize(u_lightPos - frag_pos);
	float diff = max(dot(lightDir, frag_normal), 0.0);
	vec3 diffuse = diff * u_lightColour;
		
	vec3 lighting = (ambient + (1.0f - shadow) * diffuse) * colour;

	if(u_useTexture)
	{
		outputColour = vec4(lighting, 1.0f);
	}
	else
	{
		outputColour = u_fragColour;
	}
} 