#version 430 core
in vec2 UV;
in int ftexIndex;
in vec4 ftintAlpha;

out vec4 color;

///should be array
uniform sampler2D uitex1;
uniform sampler2D uitex2;
uniform sampler2D uitex3;
uniform sampler2D uitex4;
uniform sampler2D uitex5;

void main()
{
    color = texture2D(uitex1, UV) * ftintAlpha;
}
