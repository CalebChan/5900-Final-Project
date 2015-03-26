#version 330 core

uniform mat4 modelWorldViewMat; 
uniform mat4 projMat;

in vec4 vPos;		// vertex position
in vec4 vNormal;	// vertex normal
in vec4 vColour;	// vertex colour
in vec2 vTex;			// vertex texture coordinates

varying vec2 tex1;


out Data{
	vec4 pos;
	vec4 normal;
	vec4 colour;
	vec2 tex;
}vOut;

void main(){
	gl_Position = projMat * modelWorldViewMat * vPos;
	vOut.normal = projMat * modelWorldViewMat * vNormal;
	vOut.pos = gl_Position;
	vOut.colour = vColour;
	vOut.tex = vTex;
	tex1 = vTex;
	
}