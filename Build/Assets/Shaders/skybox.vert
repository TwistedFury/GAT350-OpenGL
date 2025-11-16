#version 460 core

layout (location = 0) in vec3 a_position;

out vec3 v_texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	v_texcoord = a_position;

	// Remove translation from view matrix
	mat4 view_no_translation = mat4(mat3(u_view));  // Keep only rotation
	mat4 view_projection = u_projection * view_no_translation;

	vec4 pos = view_projection * vec4(a_position, 1.0);
	gl_Position = pos.xyww;  // Set z=w so depth is always 1.0
}
