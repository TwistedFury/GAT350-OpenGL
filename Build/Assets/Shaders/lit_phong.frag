#version 460 core

#define MAX_LIGHTS   5
#define POINT	     0
#define DIRECTIONAL  1
#define SPOT		 2

#define BASE_MAP     (1 << 0)
#define SPECULAR_MAP (1 << 1)
#define EMISSIVE_MAP (1 << 2)
#define NORMAL_MAP   (1 << 3)

in VS_OUT
{
    vec2 texcoord;
	vec3 normal;
	vec3 position;
	mat3 tbn;
} fs_in;

struct Light
{
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
    float intensity;
    float range;
	float innerSpotAngle;
	float outerSpotAngle;
};

struct Material
{
    vec3 baseColor;
	vec3 emissiveColor;
    float shininess;
    vec2 tiling;
    vec2 offset;
	uint parameters;
};

uniform int u_numLights = 1;
uniform Light u_lights[MAX_LIGHTS];
uniform Material u_material;
uniform sampler2D u_baseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_emissiveMap;
uniform sampler2D u_normalMap;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 u_ambient_light;

vec3 calculateNormal()
{
	// generate the normals from the normal map
	vec3 normal = texture(u_normalMap, fs_in.texcoord).rgb;
	// convert rgb normal (0 <-> 1) to xyx (-1 <-> 1)
	normal = normalize((normal * 2) - 1);
	// transform normals to model view space
	normal = normalize(fs_in.tbn * normal);

	return normal;
}

float calculateAttenuation(in float light_distance, in float range)
{
	float attenuation = max(0.0, 1.0 - (light_distance / range));
	return pow(attenuation, 2.0);
}

vec3 calculateLight(in Light light, in vec3 position, in vec3 normal, in float specularMask)
{
	vec3 light_dir; 
	float attenuation = 1; 
	float light_distance;
	
	switch(light.type)
	{
		case POINT:
			light_dir = normalize(light.position - position);
			light_distance = distance(light.position, position);
			attenuation = calculateAttenuation(light_distance, light.range);
		break;

		case DIRECTIONAL:
			light_dir = normalize(-light.direction);  // Fixed normalize
		break;
		
		case SPOT:
			light_dir = normalize(light.position - position);
			light_distance = distance(light.position, position);
			attenuation = calculateAttenuation(light_distance, light.range);

			float angle = acos(dot(light_dir, -light.direction));
			if (angle > light.outerSpotAngle) attenuation = 0;
			else {
				attenuation *= smoothstep(light.outerSpotAngle, light.innerSpotAngle, angle);
			}
		break;
	}
	
	// USE light_dir instead of recalculating L
	vec3 L = light_dir;  // ? Fixed!
	vec3 V = normalize(-position);

    // Diffuse (Lambert)
    float NdotL = max(dot(normal, L), 0);
    vec3 diffuse = light.color * NdotL;

	// Specular
	float specularStrength = 1.0;
	vec3 H = normalize(L + V);
	float NdotH = max(dot(normal, H), 0);
	NdotH = pow(NdotH, u_material.shininess);
	vec3 specular = specularStrength * NdotH * light.color * specularMask; 

	return (diffuse + specular) * light.intensity * attenuation;
}

out vec4 f_color;

void main()
{
	float specularMask = ((u_material.parameters & SPECULAR_MAP) != 0u) 
		? texture(u_specularMap, fs_in.texcoord).r : 1;

	// fs_in.position is ALREADY in view space from vertex shader!
	vec3 position = fs_in.position;  // Don't transform again!
	
	vec3 normal = ((u_material.parameters & NORMAL_MAP) != 0u)
		? calculateNormal()
		: fs_in.normal;

	vec3 color = u_ambient_light;
	for (int i = 0; i < u_numLights; i++)
	{
	    color += calculateLight(u_lights[i], position, normal, specularMask);
	}

	vec4 emissive = ((u_material.parameters & EMISSIVE_MAP) != 0u) 
		? texture(u_emissiveMap, fs_in.texcoord) * vec4(u_material.emissiveColor, 1)
		: vec4(u_material.emissiveColor, 1);

  // Final color: base texture modulated by lighting, plus emissive
  f_color = texture(u_baseMap, fs_in.texcoord) * vec4(color, 1) + emissive;
}