#version 460 core

uniform vec2 Position;
uniform float Radius = 0.5;
uniform vec4 Color;
uniform mat4x4 CameraMatrix = mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

layout (location = 0) in vec3 aPos;

out vec2 quadCoord;

void main()
{
    gl_Position = CameraMatrix * vec4(aPos.xy * Radius + Position, aPos.z, 1.0);
    quadCoord = vec2(aPos.x * 2, aPos.y * 2);
}