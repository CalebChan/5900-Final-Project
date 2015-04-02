#version 330 core

uniform sampler2D texHandle;

layout(location = 0) out vec4 color;

varying vec2 texLocal;

void main(){
	//color = vec4(1.0);
	//vec2 t = texLocal;
	//color = texture2D(texHandle, t); 
	
	float Depth = texture(texHandle, texLocal).x;
	Depth = 1.0 - (1.0 - Depth) * 50.0;
    	color = vec4(vec3(Depth), 1.0);
}