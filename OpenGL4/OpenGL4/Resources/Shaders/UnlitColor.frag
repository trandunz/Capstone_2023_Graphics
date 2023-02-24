#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Outside Variables Passed In As 'Uniforms'
uniform vec3 Color;

void main()
{
	FragColor = vec4(Color,1.0f);
}