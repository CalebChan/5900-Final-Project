#version 330 core

struct BaseLight                                                                    
{                                                                                   
    vec3 Color;                                                                     
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
};                                                                                  

struct DirectionalLight                                                             
{                                                                                   
    BaseLight Base;                                                                 
    vec3 Direction;                                                                 
};  




varying vec2 tex1;                                                                 
varying vec3 Normal0;                                                                    
varying vec3 WorldPos0;                                                                  

out vec4 FragColor;

uniform sampler2D texHandle;
uniform DirectionalLight dirLight;                                                 

in Data{
	vec4 pos;
	vec4 normal;
	vec4 colour;
	vec2 tex;
} vIn;

                                                                                                                                                                                                                                                           

vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)                   
{                                                                                           
    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;                   
    float DiffuseFactor = dot(Normal, -LightDirection);                                     

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                  
    vec4 SpecularColor = vec4(0, 0, 0, 0);                                                  

    if (DiffuseFactor > 0) {                                                                
        DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;                                                                                      
    }                                                                                       

    return (AmbientColor + DiffuseColor);                                   
}                                                                                           

vec4 CalcDirectionalLight(vec3 Normal)                                                      
{                                                                                           
    return CalcLightInternal(dirLight.Base, dirLight.Direction, Normal);  
}                                                                                                                                                                                  


void main()
{             
	vec3 Normal = normalize(Normal0);                                                       
    vec4 TotalLight = CalcDirectionalLight(Normal);                                         

    FragColor = texture(texHandle, tex1.xy) * TotalLight;
}