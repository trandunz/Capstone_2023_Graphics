#version 460 core

in vec2 FragTexCoords;
in vec3 FragNormal;

uniform sampler2D ImageTexture0;


uniform float AmbientStrength;
uniform vec3 AmbientColor;
uniform float Shininess;

uniform vec3 CameraPos;

out vec4 FinalColor;

void main()
{
	vec3 Ambient = AmbientStrength * AmbientColor;

	vec4 Light = vec4(Ambient, 1.0f);

	FinalColor = Light * texture(ImageTexture0,FragTexCoords);
}