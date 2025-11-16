#version 460 core

#define MAX_LIGHTS  5
#define POINT	    0
#define DIRECTIONAL 1
#define SPOT		2

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texcoord;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec3 a_tangent;

out VS_OUT
{
    vec2 texcoord;
	vec3 normal;
    vec3 position;
    mat3 tbn;
} vs_out;

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

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 u_ambient_light;

void main()
{
    vs_out.texcoord = (a_texcoord + u_material.tiling) + u_material.offset;
    // Compute world-space position and normal
    mat4 model_view = u_view * u_model;
    mat3 normal_matrix = transpose(inverse(mat3(model_view)));
	vec3 position = vec3(model_view * vec4(a_position, 1.0));

	vs_out.texcoord = (a_texcoord * u_material.tiling) + u_material.offset;
    vs_out.position = vec3(model_view * vec4(a_position, 1));
    vs_out.normal = normalize(normal_matrix * a_normal);

    vec3 N = normalize(normal_matrix * a_normal);
    vec3 T = normalize(normal_matrix * a_tangent);
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(cross(N, T));
    vs_out.tbn = mat3(T, B, N);

    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}
