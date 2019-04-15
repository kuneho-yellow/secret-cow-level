#version 130
// Operates on each vertex

// Input data from the VBO.
in vec2 VertexPosition;
in vec4 VertexColor;
in vec2 VertexUV;

out vec4 FragmentColor;
out vec2 FragmentUV;

uniform mat4 VPMat;

void main()
{
	// Set the position
	gl_Position.x = VertexPosition.x;
	gl_Position.y = VertexPosition.y;
	gl_Position.z = 0;
	gl_Position.w = 1.0;
	gl_Position = VPMat * gl_Position;
	
	FragmentColor = VertexColor;
	FragmentUV = vec2(VertexUV.x, 1.0 - VertexUV.y);
}