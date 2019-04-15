#version 130
// Operates on each fragment(pixel) in a given polygon

in vec4 FragmentColor;
in vec2 FragmentUV;

out vec4 color;

uniform sampler2D TexSampler;

void main()
{
	vec4 TextureColor = texture(TexSampler, FragmentUV);
	color = TextureColor * FragmentColor;
}