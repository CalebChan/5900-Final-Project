#version 330 core

uniform sampler2D texHandle;

out vec4 color;

varying vec2 texLocal;

void main(){
	vec2 t = texLocal;
	color = texture2D(texHandle, t); 
}