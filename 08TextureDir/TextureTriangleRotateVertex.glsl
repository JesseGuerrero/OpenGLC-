#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 gRotation;

void main()
{
    mat4 x = gRotation;
    gl_Position = 1*vec4(aPos.x, aPos.y / 2, aPos.z, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}