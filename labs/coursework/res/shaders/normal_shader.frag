#version 450 core
// This shader requires direction.frag and normal_map.frag

// Point light information
#ifndef POINT_LIGHT
#define POINT_LIGHT
struct point_light
{
    vec4 light_colour;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};
#endif

// Spot light data
#ifndef SPOT_LIGHT
#define SPOT_LIGHT
struct spot_light
{
    vec4 light_colour;
    vec3 position;
    vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float power;
};
#endif

// A material structure
#ifndef MATERIAL
#define MATERIAL
struct material
{
    vec4 emissive;
    vec4 diffuse_reflection;
    vec4 specular_reflection;
    float shininess;
};
#endif

// Forward declarations of used functions
vec4 calculate_point(in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir,
                     in vec4 tex_colour);
vec4 calculate_spot(in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir,
                    in vec4 tex_colour);
vec3 calc_normal(in vec3 normal, in vec3 tangent, in vec3 binormal, in sampler2D normal_map, in vec2 tex_coord);


uniform point_light points[4];
// Spot lights being used in the scene
uniform spot_light spots[2];
// Material of the object being rendered
uniform material mat;
// Position of the eye
uniform vec3 eye_pos;
// Texture to sample from
uniform sampler2D tex;
// Normal map to sample from
uniform sampler2D normal_map;

// Incoming vertex position
layout(location = 0) in vec3 position;
// Incoming texture coordinate
layout(location = 1) in vec2 tex_coord;
// Incoming normal
layout(location = 2) in vec3 normal;
// Incoming tangent
layout(location = 3) in vec3 tangent;
// Incoming binormal
layout(location = 4) in vec3 binormal;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main()
{
    // *********************************
    // Sample texture
    vec4 tex_colour = texture(tex, tex_coord);
    // Calculate view direction
    vec3 view_dir = normalize(eye_pos - position);

    // Calculate normal from normal map
    vec3 new_normal = calc_normal(normal, tangent, binormal, normal_map, tex_coord);
    for (int i = 0; i < points.length(); i++)
    {
        colour += calculate_point(points[i], mat, position, new_normal, view_dir, tex_colour);
    }

    // Sum spot lights
    for (int j = 0; j < spots.length(); j++)
    {
        colour += calculate_spot(spots[j], mat, position, new_normal, view_dir, tex_colour);
    }
    // *********************************
}