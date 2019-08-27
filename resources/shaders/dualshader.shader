#shader vertex
#version 410 core

// layout(location 0) : the 0 needs to match the index in glVertexAttribPointer
// in : this is input data
// vec3 : parameter being given to main is a vec3
// vertexPosition_modelspace : local variable name to hold the position of the vertex
//layout(location = 0) in vec4 vertexPosition_modelspace;

layout(location = 0)
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
in  vec3 in_Position;
in  vec4 in_Color;
uniform mat4 u_MVP;

// We output the ex_Color variable to the next shader in the chain
out vec4 ex_Color;

void main() {

	gl_Position = u_MVP * vec4(in_Position.x, in_Position.y, 0.0, 1.0);
	//gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
	ex_Color = in_Color;
}


#shader fragment
#version 410 core

layout(location = 1)

// In data
in vec4 ex_Color;

// Ouput data
out vec4 out_color;
uniform vec4 u_colour;


void main()
{
	//out_color = u_colour;
	out_color = ex_Color;
}