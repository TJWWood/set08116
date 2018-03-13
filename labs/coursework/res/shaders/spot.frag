// Spot light calculation
vec4 calculate_spot(in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour)
{
    // *********************************
    // Calculate direction to the light
    vec3 light_dir = normalize(spot.position - position);
    // Calculate distance to light
    float d = distance(position, spot.position);

    // Calculate attenuation value
    float atten_fac = spot.constant + (spot.linear * d) + (spot.quadratic * d * d);

    // Calculate spot light intensity
    float intensity = pow(max(dot(-spot.direction, light_dir), 0.0), spot.power);

    // Calculate light colour
    vec4 light_colour = (intensity / atten_fac) * spot.light_colour;

    //Standard phong shading
    float k = max(dot(normal, light_dir), 0.0);
    vec4 diffuse = k * (mat.diffuse_reflection * light_colour);
    vec3 half_vec = normalize(light_dir + view_dir);
    k = pow(max(dot(normal, half_vec), 0.0), mat.shininess);
    vec4 specular = k * (mat.specular_reflection * light_colour);
    vec4 primary = mat.emissive + diffuse;
    vec4 colour = primary * tex_colour + specular;
    colour.a = 1.0;

    return colour;
}