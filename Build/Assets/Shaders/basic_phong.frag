#version 460 core

in VS_OUT
{
    vec2 texcoord;
    vec3 color;
} vs_out;

void calculateLight()
{

}

out vec4 f_color;

void main()
{
    f_color = vs_out.color;
}