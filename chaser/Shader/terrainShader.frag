#version 330 core

uniform sampler2D texHandle;
uniform sampler2D shadowMap;

//layout(location = 0) out float color;
layout(location = 0) out vec4 color;

varying vec4 position;
varying vec3 normals;
varying vec2 texLocal;

void main(){
	//color = vec4(1.0);
	vec2 t = texLocal;
	color = texture2D(texHandle, t); 
	//color = gl_FragCoord.z;
	
	float vis = 1.0;
	color = color * vis;
}