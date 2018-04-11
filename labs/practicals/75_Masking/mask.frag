#version 430 core

// Captured render
uniform sampler2D tex;
// Alpha map
uniform sampler2D alpha_map;

// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
    // *********************************
    // Sample textures
    tex = texture(colour, tex_coord);
    alpha_map = texture(colour, tex_coord);
    // Final colour is produce of these two colours
    colour = tex * alpha_map;
    // Ensure alpha is 1
    colour.a = 1.0;
  // *********************************
}