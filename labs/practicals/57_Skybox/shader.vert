#version 440

// The transformation matrix
uniform mat4 MVP;

// Incoming position
layout(location = 0) in vec3 position;

layout(location = 0) out vec3 tex_coord;


void main()
{
    // Transform the position into screen space
    gl_Position = MVP * vec4(position, 1.0);
    tex_coord = position;
}