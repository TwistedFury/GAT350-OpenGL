#version 460 core

in vec3 v_color;
out vec4 f_color;

uniform float u_time;

void main()
{
	//f_color = vec4(1, 0, 0, 1);
	float y_offset = sin(u_time + gl_FragCoord.y);
	float x_offset = cos(u_time + gl_FragCoord.x);
	f_color = vec4(v_color % (y_offset * x_offset), 1);
}