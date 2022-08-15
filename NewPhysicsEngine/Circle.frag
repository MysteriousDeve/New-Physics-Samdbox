#version 460 core

uniform vec4 Color;

in vec4 gl_FragCoord;
out vec4 FragColor;

in vec2 quadCoord;

void main()
{
    if ((pow(quadCoord.x, 2) + pow(quadCoord.y, 2)) > 1) discard;

    FragColor = Color;
}