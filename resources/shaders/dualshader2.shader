#shader vertex
#version 410 core

// INPUT VARIABLES 
in vec3 input_position;
in vec3 input_colour;

// OUTPUT VARIABLES 
out vec3 pass_colour;

// UNIFORM VARIABLES 
uniform mat4 u_mvp_matrix;

// Vertex Shader
void main(void)
{
	// Set the vertex position to the mvp matrix
	gl_Position = u_mvp_matrix * vec4(input_position.x, input_position.y, input_position.z, 1.0f);

	// Store the input color for the pixel shader to use.
	pass_colour = input_colour;
}

/////////////////////////////////////////////////////////
#shader fragment
#version 410 core

// INPUT VARIABLES 
in vec3 pass_colour;

// OUTPUT VARIABLES 
out vec4 out_colour;

// UNIFORM VARIABLES
uniform uint solid_colour_mode;

// Pixel Shader
void main(void)
{
	uint flag = solid_colour_mode;
	if (flag == 1)
	{
		out_colour = vec4(0.5f, 0.8f, 0.5f, 1.0f);
	}
	else
	{
		out_colour = vec4(pass_colour, 1.0f);
	}
}