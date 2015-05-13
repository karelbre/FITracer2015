/*
	Name: FShader.glsl
	Desc: Fragment shader for render result PT and others.
	Author: Karel Brezina (xbrezi13)
*/

#version 140

in vec2 ex_Texcoord;

out vec4 out_Color;

uniform bool uni_isBorder;
uniform vec4 uni_Color;
uniform sampler2D uni_Sampler;
uniform float uni_MixTexCol;

void main()
{
	if (uni_isBorder)
		out_Color = vec4(0.2f, 0.2f, 0.2f, 1.f);
	else { 
		vec4 tex = texture(uni_Sampler, ex_Texcoord);	
		out_Color = mix(tex, uni_Color, uni_MixTexCol);
	}
}