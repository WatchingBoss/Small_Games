#version 330 core

layout(location = 0) out vec4 color;

in vec2 TextureCoor;

uniform sampler2D Texture1;

void main()
{
	color = texture(Texture1, TextureCoor);
}
