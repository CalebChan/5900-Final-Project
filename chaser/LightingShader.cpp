#include "LightingShader.h"

void LightingShader::setLightPosition(Vector3f pos){
	glUniform3f(directionLocations.Direction, pos.x, pos.y, pos.z);
}

void LightingShader::setAmbientIntensity(float value){
	glUniform1f(directionLocations.AmbientIntensity, value);
}

void LightingShader::setDiffiuseIntensity(float value){
	glUniform1f(directionLocations.DiffuseIntensity, value);
}

void LightingShader::setLightColor(Vector3f color){
	glUniform3f(directionLocations.Color, color.x, color.y, color.z);
}

void LightingShader::setDirectionalLight(DirectionalLight &light){
	setLightColor(light.Color);
	setLightPosition(light.Direction);
	setAmbientIntensity(light.AmbientIntensity);
	setDiffiuseIntensity(light.DiffuseIntensity);
}

void LightingShader::init(){
	directionLocations.Color = glGetUniformLocation(this->getProgId(), "dirLight.Base.Color");
	directionLocations.AmbientIntensity = glGetUniformLocation(this->getProgId(), "dirLight.Base.AmbientIntensity");
	directionLocations.DiffuseIntensity = glGetUniformLocation(this->getProgId(), "dirLight.Base.DiffuseIntensity");
	directionLocations.Direction = glGetUniformLocation(this->getProgId(), "dirLight.Direction");

	setLightPosition(Vector3f(0.0, 0.0, 0.0));
	setLightColor(Vector3f(1.0, 1.0, 1.0));
	setAmbientIntensity();
	setDiffiuseIntensity();
}