#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 2) in vec3 a_normal;

out vec3 v_texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	vec3 position = vec3(u_model * vec4(a_position, 1));
	vec3 normal = normalize(mat3(transpose(inverse(u_model))) * a_normal);

	vec3 view_position = inverse(u_view)[3].xyz;
	vec3 view_dir = normalize(position - view_position);

	v_texcoord = reflect(view_dir, normal);

	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}
