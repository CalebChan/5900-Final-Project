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

layout(location = 0) out vec4 color;

float LinearizeDepth(float depth)
{
    float near = 0.1; 
    float far = 100.0; 
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near) / (far + near - z * (far - near));	
}

void main()

{

	//color = vIn.colour; 
	vec2 t = tex1;
	vec4 texColor = texture2D(texHandle, tex1);
//	gl_FragColor = color; 	

	vec4 shadowCoordinateWdivide = shadowCoor / shadowCoor.w ;
	// Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0005;
	float distanceFromLight = texture2D(shadowMap, t).z;
	float shadow = 1.0;
	if (shadowCoor.w > 0.0)
		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
		
	color = texture2D(texHandle, tex1) * shadow;
}