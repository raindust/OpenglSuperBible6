#version 430 core

layout (location = 0 ) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 vs_color;

void main()
{
    gl_Position = vec4(position.xyz, 1.0);
	vs_color = vec4(color.xyz, 1.0);
}
