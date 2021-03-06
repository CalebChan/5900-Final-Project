#version 330 core

uniform sampler2D texHandle;

in Data{
	vec4 pos;
	vec4 normal;
	vec4 colour;
	vec2 tex;
} vIn;

varying vec2 tex1;

out vec4 color;

void main()
{             
	color = texture2D(texHandle, tex1);
	//color = vec4(1.0, 0.0, 0.0, 1.0);
}
