#pragma once

#include "Shader.h"

struct BaseLight
{
	Vector3f Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight()
	{
		Color = Vector3f(0.0f, 0.0f, 0.0f);
		AmbientIntensity = 0.0f;
		DiffuseIntensity = 0.0f;
	}
};

struct DirectionalLight : public BaseLight
{
	Vector3f Direction;

	DirectionalLight()
	{
		Direction = Vector3f(0.0f, 0.0f, 0.0f);
	}
};

class LightingShader : public Shader{
public:
	void setLightPosition(Vector3f pos);
	void setAmbientIntensity(float value = 1.0);
	void setDiffiuseIntensity(float value = 1.0);
	void setLightColor(Vector3f color);

	void setDirectionalLight(DirectionalLight &light);

	void init();

private:
	DirectionalLight light;
	struct{
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Direction;
	}directionLocations;
};