#version 330 core

uniform sampler2D texHandle;
//uniform sampler2DShadow shadowMap;
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
	vec4 shadowPos = shadowCoor / shadowCoor.w;
	
	float distanceFromLight = texture2D(shadowMap, shadowPos.st).z;
	float shadow = 1.0;
	if (shadowCoor.w > 0){
		if (distanceFromLight < shadowPos.z - 0.0005){
			shadow = 0.5;
		}
	}
	vec4 ModelColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	color = shadow * ModelColor;
	
	/*float shadow = textureProj(shadowMap, shadowCoor);
    color = texture2D(texHandle, tex1) * shadow;*/
}