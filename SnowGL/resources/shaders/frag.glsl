#version 430 core

uniform sampler2D u_texture;
uniform vec4 u_diffuse = { 1.0f, 0.78f, 0.91f, 1.0f };
uniform bool u_useTexture = false;
//uniform vec3 u_cameraPosition;
uniform mat4 u_m;

uniform float u_materialShininess = 0.1f;
uniform vec3 u_materialSpecularCol = { 1.0f, 1.0f, 1.0f };

uniform vec3 u_lightPosition = { 0, 0, 2 };

layout(location = 0) out vec4 color;

in vec2 frag_texCoord;
in vec3 frag_vert;
in vec3 frag_normal;

void main()
{
	float ambientCoefficient	= 0.5f;
	float intensities			= 0.9f;
	float lightAttenuation		= 1.0f;

	vec4 surfaceColor;
	if(u_useTexture)
	{
		surfaceColor = texture(u_texture, frag_texCoord);
	}
	else
	{
		surfaceColor = u_diffuse;
	}

	
	vec3 ambient = ambientCoefficient * surfaceColor.rgb * intensities;

	//calculate normal in world coordinates
    mat3 normalMatrix = transpose(inverse(mat3(u_m)));
    vec3 normal = normalize(normalMatrix * frag_normal);
    
    //calculate the location of this fragment (pixel) in world coordinates
    vec3 fragPosition = vec3(u_m * vec4(frag_vert, 1));
    
    //calculate the vector from this pixels surface to the light source
    vec3 surfaceToLight = u_lightPosition - fragPosition;

    //calculate the cosine of the angle of incidence
    float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);

    //calculate final color of the pixel, based on:
    // 1. The angle of incidence: brightness
    // 2. The color/intensities of the light: light.intensities
    // 3. The texture and texture coord: texture(tex, fragTexCoord)
    
    color = vec4((brightness * intensities * surfaceColor.rgb) + ambient, surfaceColor.a);
	
	//final color (after gamma correction)
    //vec3 gamma = vec3(1.0/2.2);
    //color = vec4(pow(linearColor, gamma), surfaceColor.a);
} 