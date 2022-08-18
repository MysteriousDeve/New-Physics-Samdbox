#version 460 core

uniform vec4 Color;

in vec4 gl_FragCoord;
out vec4 FragColor;

void main()
{
    FragColor = Color;
}