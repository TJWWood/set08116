// Calculates the directional light
vec4 calculate_direction(in directional_light light, in material mat, in vec3 normal, in vec3 view_dir, in vec4 tex_colour)
{
 // *********************************
	// Calculate ambient component
vec4 ambient = light.ambient_intensity + light.light_colour;
	// Calculate diffuse component :  (diffuse reflection * light_colour) *  max(dot(normal, light direction), 0)
	vec4 diffuse = mat.diffuse_reflection * light.light_colour;
	// Calculate normalized half vector 
vec3 half_vec = normalize(light.light_dir + view_dir);
	// Calculate specular component : (specular reflection * light_colour) * (max(dot(normal, half vector), 0))^mat.shininess
vec4 specular = (mat.specular_reflection * light.light_colour) * pow(max(dot(normal, half_vec), 0.0), mat.shininess);
 // *********************************
	// Calculate colour to return
	vec4 colour = ((mat.emissive + ambient + diffuse) * tex_colour) + specular;
	colour.a = 1.0;
	// Return colour
	return colour;
}
