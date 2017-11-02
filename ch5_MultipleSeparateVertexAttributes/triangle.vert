#version 430 core

layout (location = 0 ) in vec4 position;
layout (location = 1) in vec4 color;

out vec4 vs_color;

void main()
{
    gl_Position = position;
	vs_color = color;
}
