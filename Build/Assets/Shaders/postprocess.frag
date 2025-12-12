#version 430

#define GRAYSCALE	(1 << 0)
#define COLORTINT	(1 << 1)

in vec2 v_texcoord;

out vec4 f_color;

// post process controls
uniform uint u_parameters = 0;
uniform vec3 u_colorTint = vec3(0.5, 0.5, 0.5);

uniform float u_time = 0;
uniform float u_intensity = 1.0;

uniform sampler2D u_baseMap;

void main()
{
	vec4 color = texture(u_baseMap, v_texcoord);

	vec4 postprocess = color;
	// Apply PostFX
	//if ((u_parameters & COLORTINT) != 0) postprocess += u_colorTint;
	//if ((u_parameters & GRAYSCALE) != 0) applyGrayScale();
	
	//f_color = postprocess;
}