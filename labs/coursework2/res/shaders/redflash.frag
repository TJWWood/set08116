#version 430 core

// Incoming texture containing frame information
uniform sampler2D tex;

// Our colour filter - calculates colour intensity
const vec3 intensity = vec3(0.895, 0.587, 0.184);

// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Sample texture colour
  vec4 col = texture(tex, tex_coord);
  // Calculate red value
  //col = dot(col, intensity);
  // Use red as final colour
  // - ensure alpha is 1
  colour = col;
  colour.a = 1;
  // *********************************
}