#version 430 core
in vec2 UV;
in vec4 tintAlpha;
in int texIndex;

out vec4 color;

uniform sampler2D textureSampler;

void main()
{
	color = texture2D(textureSampler, UV);
}