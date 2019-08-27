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

// We output the ex_Color variable to the next shader in the chain
out vec4 ex_Color;

void main(){

	gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
	ex_Color = in_Color;
}
