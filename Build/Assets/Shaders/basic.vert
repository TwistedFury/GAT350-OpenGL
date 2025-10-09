#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
//layout (location = 2) in vec2 a_uv;

out vec3 v_color;
uniform float u_time;

void main()
{
	v_color = a_color;
	gl_Position = vec4(sin(u_time + a_position), 1.0f); // That's fun looking
}
