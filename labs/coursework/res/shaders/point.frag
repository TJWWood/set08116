
// Point light calculation
vec4 calculate_point(in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour)
{
    // *********************************
    // Get distance between point light and vertex
    float d = distance(position, point.position);

    // Calculate attenuation factor
    float atten_fac = 1 / (point.constant + (point.linear * d) + (point.quadratic * d * d));

    // Calculate light colour
    vec4 light_colour = point.light_colour * atten_fac;
    light_colour.a = 1.0;
    // Calculate light dir
    vec3 light_dir = normalize(point.position - position);


    //Standard phong shading
    float k = max(dot(normal, light_dir), 0.0);
    vec4 diffuse = k * (mat.diffuse_reflection * light_colour);
    vec3 half_vec = normalize(light_dir + view_dir);
    k = pow(max(dot(normal, half_vec), 0.0), mat.shininess);
    vec4 specular = k * (mat.specular_reflection * light_colour);
    vec4 primary = mat.emissive + diffuse;
    vec4 colour = primary * tex_colour + specular;
    colour.a = 1;
    // *********************************
    return colour;
}