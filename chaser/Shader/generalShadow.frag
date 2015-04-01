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
	vec4 shadowCoordinateWdivide = shadowCoor / shadowCoor.w ;
		
	// Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0005;
	
	float distanceFromLight = texture2D(shadowMap,shadowCoordinateWdivide.st).z;
	
	float shadow = 1.0;
	if (shadowCoor.w > 0.0)
		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
	
	
	color =	 shadow * texture2D(texHandle, tex1);
}