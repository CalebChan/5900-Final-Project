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
	//color = texture2D(texHandle, t) * shadowCoor.w;
//	gl_FragColor = color; 	

	vec4 LightColor = vec4(1,1,1,1);
	vec4 MaterialDiffuseColor = vec4(texture2D( texHandle, t ).rgb, 1);
	float visibility = 1.0;
	if (texture2D(shadowMap, t).z > shadowCoor.z){
		visibility = 0.5;
	}
	color = visibility * MaterialDiffuseColor * LightColor;
}