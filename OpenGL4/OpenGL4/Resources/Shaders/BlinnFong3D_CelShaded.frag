#version 460 core

in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPosition;

#define MAX_POINT_LIGHTS 4
#define MAX_COLOR_TONES 3

struct PointLight
{
    vec3 Position;
    vec3 Color;
    float SpecularStrength;

    float AttenuationLinear;
    float AttenuationExponent;
};

uniform int TextureCount;
uniform sampler2D ImageTexture0;

uniform vec3 CameraPos;
uniform vec3 AmbientColor;
uniform float Shininess;
uniform float AmbientStrength;

uniform int PointLightCount;

uniform PointLight PointLights[MAX_POINT_LIGHTS];

out vec4 FinalColor;

vec3 CalculatePointLight(PointLight _pointLight);
vec3 CalculateAmbientLight();
vec3 ApplyCellShading(vec3 _lighting, vec3 _lightDir);

vec3 ReverseViewDir;

void main()
{
    ReverseViewDir = normalize(CameraPos - FragPosition);

    vec3 combinedLighting = CalculateAmbientLight();
    for (int i = 0; i < MAX_POINT_LIGHTS && i < PointLightCount; i++)
    {
        combinedLighting += CalculatePointLight(PointLights[i]);
    }

    if (TextureCount > 0)
    {
	    FinalColor = vec4(combinedLighting,1.0f) * texture(ImageTexture0,FragTexCoords);
    }
    else
    {  
        FinalColor = vec4(combinedLighting,1.0f);
    }
}

vec3 CalculateAmbientLight()
{
    return AmbientStrength * AmbientColor;
}

vec3 CalculatePointLight(PointLight _pointLight)
{
    vec3 lightDir = normalize(FragPosition - _pointLight.Position);

    float strength = max(dot(FragNormal, -lightDir), 1.0f);
    vec3 diffuseLight = strength * _pointLight.Color.rgb;

    vec3 halfwayVector = normalize(-lightDir + ReverseViewDir);
    float specularReflectivity = pow(max(dot(FragNormal, halfwayVector), 0.0f), Shininess);
    vec3 specularLight = _pointLight.SpecularStrength * specularReflectivity * _pointLight.Color.rgb;

    float distance = length(_pointLight.Position - FragPosition);
    float attenuation = 1 + (_pointLight.AttenuationLinear * distance) + (_pointLight.AttenuationExponent * pow(distance, 2.0f));
    return ApplyCellShading((diffuseLight + specularLight) / attenuation, lightDir);
}

vec3 ApplyCellShading(vec3 _lighting, vec3 _lightDir)
{
    float nl = dot(FragNormal, -_lightDir);

    if(nl > 0.5f)
    {
        return _lighting * 0.6f; 
    }
    else
    {
        return _lighting * 0.35f;
    }
}