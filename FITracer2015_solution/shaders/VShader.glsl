/*
	Name: VShader.glsl
	Desc: Vertex shader.
	Author: Karel Brezina (xbrezi13)
*/

#version 140

in vec2 in_Position;
in vec2 in_Texcoord;

out vec2 ex_Texcoord;

void main()
{
	gl_Position = vec4(in_Position.xy, 0.0, 1.0);
	ex_Texcoord = in_Texcoord;
}