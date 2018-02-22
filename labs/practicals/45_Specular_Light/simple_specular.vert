#version 440

// The model matrix
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;
// Material colour
uniform vec4 material_colour;
// Shininess of the object
uniform float shininess;
// Light colour
uniform vec4 light_colour;
// Direction of the light
uniform vec3 light_dir;
// Position of the camera
uniform vec3 eye_pos;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 2) in vec3 normal;

// Outgoing vertex colour
layout(location = 0) out vec4 vertex_colour;

void main() {
  // *********************************
  // Calculate position
   gl_Position = MVP * vec4(position, 1.0);
  // Transform the normal
  N = normalize(MVP * normal);
  // Calculate world position

  // Calculate view direction
  vec3 view_direction = normalize(eye_pos - world_pos)
  // Calculate half vector between view_dir and light_dir
  vec4 half_vec = (light_dir * eye_pos) / 2;
  // Calculate k
   float k = max(dot((N*normal), light_dir), 0.0);
  // Calculate specular
  vec4 specular = max(dot(normal * half_vec));
  // Ensure alpha is 1
  specular.a = 1.0;
  // Output vertex colour - just diffuse
  vertex_colour = specular;
  // *********************************
}