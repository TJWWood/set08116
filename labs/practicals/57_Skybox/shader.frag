#version 440

layout(location = 0) out vec4 colour;
// Cubemap data
uniform samplerCube cubemap;

// Incoming 3D tex_coord
layout(location = 0) in vec3 tex_coord;

void main()
{
    colour = texture(cubemap, tex_coord);
}