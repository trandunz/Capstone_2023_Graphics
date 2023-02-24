#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec3 l_position;

// Outside Variables Passed In As 'Uniforms'
uniform mat4 PVMMatrix;

void main()
{
	gl_Position = PVMMatrix * vec4(l_position,1.0f);
}