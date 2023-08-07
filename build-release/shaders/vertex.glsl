#version 460

layout (location = 0) in vec3 pos;

void main()
{
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
    vec4 scale = vec4(0.5, 0.5, 0.5, 1.0);
    gl_Position *= scale;
}