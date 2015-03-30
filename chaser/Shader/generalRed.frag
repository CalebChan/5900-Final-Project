#version 330 core

uniform sampler2D texHandle;

in Data{
	vec4 pos;
	vec4 normal;
	vec4 colour;
	vec2 tex;
} vIn;

varying vec2 tex1;
varying vec4 shadowCoor;

out vec4 color;

void main()

{

	color = vIn.colour * shadowCoor.w; 
	vec2 t = tex1;
//	color = texture2D(texHandle, t);
//	gl_FragColor = color; 	

}