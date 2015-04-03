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
	vec2 poissonDisk[4] = vec2[](
		vec2( -0.94201624, -0.39906216 ),
		vec2( 0.94558609, -0.76890725 ),
		vec2( -0.094184101, -0.92938870 ),
		vec2( 0.34495938, 0.29387760 ));


	vec4 shadowPos = shadowCoor / shadowCoor.w;
	
	float distanceFromLight = texture2D(shadowMap, shadowPos.xy).z;
	float shadow = 1.0;
	for (int i = 0; i < 4; i++){
		if (texture2D(shadowMap, shadowPos.xy + poissonDisk[i]/700.0).z < shadowPos.z - 0.00005){
			shadow = 0.0;
		}
	}

	vec4 ModelColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	color = shadow * ModelColor;
}