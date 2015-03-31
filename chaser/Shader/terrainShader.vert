#version 330 core

uniform mat4 modelWorldViewMat; 
uniform mat4 projMat;

in vec4 vPos;		// vertex position
in vec4 vNormal;	// vertex normal
in vec4 vColour;	// vertex colour
in vec2 vTex;		// vertex texture coordinates

varying vec2 texLocal;

void main(){
	gl_Position = projMat * modelWorldViewMat * vPos;
	texLocal = vTex;
}