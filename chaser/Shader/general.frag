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
//layout(location = 0) out float fragmentdepth;

float LinearizeDepth(float depth)
{
    float near = 0.1; 
    float far = 100.0; 
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near) / (far + near - z * (far - near));	
}

void main()
{             
    //float depth = LinearizeDepth(gl_FragCoord.z);
    //color = vec4(depth);
	color = texture2D(texHandle, tex1);
}