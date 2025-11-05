#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

out VS_OUT
{
    vec2 texcoord;
    vec3 color;
} vs_out;

struct Light
{
	vec3 position;
	vec3 color;
    float intensity;
    float range;
};

struct Material
{
    sampler2D baseMap;
    vec3 baseColor;

    float shininess;
    vec3 tiling;
    vec2 offset;
};

uniform int u_numLights = 1;
uniform Light u_lights[numLights];
uniform Material u_material;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 u_ambient_light;

float calculateAttenuation(in float light_distance, in float range)
{
	float attenuation = max(0.0, 1.0 - (light_distance / range));
	return pow(attenuation, 2.0);
}

vec3 calculateLight(in Light light, in vec3 position, in vec3 normal)
{
	vec3 L = normalize(light.position - position);
	vec3 V = normalize(u_cameraPos - position);

	// Diffuse
	float intensity = max(dot(L, normal), 0);
	vec3 diffuse = light.color * intensity;

	// Specular
	float specularStrength = 1.0;
	vec3 halfway_dir = normalize(L + V);
	intensity = max(dot(normal, halfway_dir), 0);
	intensity = pow(intensity, 32);
	vec3 specular = specularStrength * intensity * light.color; 

	float light_distance = distance(light.position, position);
	float attenuation = calculateAttenuation(light_distance, light.range);

	return (diffuse + specular) * light.intensity * attenuation;
}

void main()
{
    vs_out.texcoord = a_texcoord + u_material.tiling... // Ask nick or smth
    // Compute world-space position and normal
    mat4 model_view = u_view * u_model;
	vec3 position = vec3(model_view * vec4(a_position, 1.0);
	vec3 normal = noramlize(model_view) * a_normal;

	vs_out.color = u_ambient_light;
	for (int i = 0; i < u_numLights; i++)
	{
		vs_out.color += calculateLight(u_light[i], position, normal);
	}

    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}
