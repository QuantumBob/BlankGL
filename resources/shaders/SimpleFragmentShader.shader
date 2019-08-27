#version 410 core

layout (location = 1)

// In data
in vec4 ex_Color;

// Ouput data
out vec4 out_color;
//uniform vec4 u_colour;

void main()
{
	//out_color = u_colour;
	out_color = ex_Color;
}