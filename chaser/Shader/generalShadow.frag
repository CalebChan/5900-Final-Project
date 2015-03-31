#version 330 core

uniform sampler2D texHandle;
uniform sampler2D shadowMap;

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

	//color = vIn.colour; 
	vec2 t = tex1;
	color = texture2D(texHandle, t);
//	gl_FragColor = color; 	

	vec4 shadowCoordinateWdivide = shadowCoor / shadowCoor.w ;
	// Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0005;
	float distanceFromLight = texture2D(shadowMap, t).z;
	float shadow = 1.0;
	if (shadowCoor.w > 0.0)
		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
		
	color = color * shadow;
}