#version 440

// Point light information
struct point_light {
  vec4 light_colour;
  vec3 position;
  float constant;
  float linear;
  float quadratic;
};

// Material information
struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};

// Point light for the scene
uniform point_light point;
// Material for the object
uniform material mat;
// Eye position
uniform vec3 eye_pos;
// Texture
uniform sampler2D tex;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 1) in vec3 normal;
// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Get distance between point light and vertex
  float d = distance(point.position, position);
  // Calculate attenuation factor
  float atten_fac = 1.0 / ( point.constant + point.linear*d + point.quadratic * d*d);
  // Calculate light colour
  vec4 light_colour = (vec4(1.0f, 0.0f,0.0f, 1.0f));

  // Calculate light dir
  vec3 light_dir = normalize(position - point.position);
  // Now use standard phong shading but using calculated light colour and direction
  // - note no ambient

 float k = max(dot(normal, light_dir), 0.0);
  // Calculate diffuse
  vec4 diffuse = k * (mat.diffuse_reflection * point.light_colour);
 
  // Calculate view direction
  vec3 view_dir = normalize(eye_pos - position);

  // Calculate half vector
  vec3 half_vec = normalize(light_dir + view_dir);
  k = pow(max(dot(normal, half_vec), 0.0), mat.shininess);
  // Calculate specular
  vec4 specular = k * (mat.specular_reflection * point.light_colour);

  // Sample texture
  vec4 tex_colour = texture(tex, tex_coord);
  // Calculate primary colour component
  vec4 primary = mat.emissive + (atten_fac * (diffuse + specular));
    // Calculate final colour - remember alpha
    colour = primary * tex_colour;
  colour.a = 1.0f;
  // *********************************
}