#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 gWVP;
uniform mat4 gWorld;
uniform vec3 gCenter;


out vec3 Texcoord0;
out vec3 Normal0;
out vec3 WorldPos0;
out vec3 Center;

void main()
{
    gl_Position = gWVP * vec4(Position, 1);
    Center=(gWorld * vec4(gCenter, 1)).xyz;

    Texcoord0 = TexCoord;
    Normal0 = (gWVP * vec4(Normal, 0.0)).xyz;
    WorldPos0 = (gWorld * vec4(Position, 1.0)).xyz;
}
