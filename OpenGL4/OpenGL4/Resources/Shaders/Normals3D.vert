#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normals;

uniform mat4 PVMMatrix;
uniform mat4 ModelMatrix;

out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPosition;

void main()
{
	gl_Position = PVMMatrix * vec4(Position, 1.0f);

	FragTexCoords = TexCoords;
	FragNormal = normalize(mat3(transpose(inverse(ModelMatrix))) * Normals);
	FragPosition = vec3(ModelMatrix * vec4(Position, 1.0f));
}