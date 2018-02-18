#version 440

uniform mat4 MVP;

// *********************************
// Declare incoming values
// 0 - position
position vec2 (0.0f, 0.0f);
// 10 - tex_coord_in
tex_coord_in vec2 (10.0f, 0.0f);
// *********************************

// Outgoing value
layout (location = 0) out vec2 tex_coord_out;

void main()
{
	// Transform the position onto screen
	gl_Position = MVP * vec4(position, 1.0);
	// Output texture coordinates
	tex_coord_out = tex_coord_in;
} 