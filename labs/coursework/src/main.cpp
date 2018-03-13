#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
map<string, mesh> normal_meshes;
std::array<mesh, 3> h_meshes;
std::array<texture, 3> textures;
effect eff;
effect shadow_eff;
effect normal_map_eff;
texture tex;
texture normal_map;
target_camera cam;
free_camera free_cam;
arc_ball_camera arc_cam;
chase_camera chase_cam;
vector<point_light> points(4);
vector<spot_light> spots(2);
directional_light d_light;

shadow_map shadow;

double cursor_x = 0.0;
double cursor_y = 0.0;

bool initialise() {
	// *********************************
	// Set input mode - hide the cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Capture initial mouse position
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	// *********************************
	return true;
}

bool load_content() {

	//Initialise a new shadow map
	shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());

	// Create scene
	meshes["plane"] = mesh(geometry_builder::create_plane());
	meshes["box"] = mesh(geometry_builder::create_box());
	meshes["tetra"] = mesh(geometry_builder::create_tetrahedron());
	meshes["pyramid"] = mesh(geometry_builder::create_pyramid());
	meshes["disk"] = mesh(geometry_builder::create_disk(20));
	meshes["torus"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 7.0f));

	//Create meshes to use with normal map
	normal_meshes["cylinder"] = mesh(geometry_builder::create_cylinder(20, 20));
	normal_meshes["nSphere"] = mesh(geometry_builder::create_sphere(20,20));

	//Creathe meshes for transform hierarchy
	//main sphere
	h_meshes[0] = mesh(geometry_builder::create_sphere(20,20));
	//sphere2
	h_meshes[1] = mesh(geometry_builder::create_sphere(20, 20));
	//sphere3
	h_meshes[2] = mesh(geometry_builder::create_sphere(20, 20));


	//Transform hierarchy meshes
	h_meshes[0].get_transform().position = vec3(0.0f, 10.0f, 0.0f);
	h_meshes[0].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	h_meshes[1].get_transform().scale = vec3(0.5f, 0.5f, 0.5f);
	h_meshes[1].get_transform().position = vec3(0.0f, 0.0f, 4.0f);
	h_meshes[2].get_transform().scale = vec3(1.5f, 1.5f, 1.5f);
	h_meshes[2].get_transform().position = vec3(0.0f, 0.0f, -14.0f);

	// Transform objects
	meshes["box"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["box"].get_transform().translate(vec3(-10.0f, 2.5f, -30.0f));
	meshes["tetra"].get_transform().scale = vec3(4.0f, 4.0f, 4.0f);
	meshes["tetra"].get_transform().translate(vec3(-30.0f, 10.0f, -10.0f));
	meshes["pyramid"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["pyramid"].get_transform().translate(vec3(-10.0f, 7.5f, -30.0f));
	meshes["disk"].get_transform().scale = vec3(3.0f, 1.0f, 3.0f);
	meshes["disk"].get_transform().translate(vec3(-10.0f, 11.5f, -30.0f));
	meshes["disk"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));

	normal_meshes["cylinder"].get_transform().scale = vec3(5.0f, 10.0f, 5.0f);
	 normal_meshes["cylinder"].get_transform().translate(vec3(0.0f, 2.5f, -25.0f));

	normal_meshes["nSphere"].get_transform().scale = vec3(2.0f, 2.0f, 2.0f);
	normal_meshes["nSphere"].get_transform().translate(vec3(10.0f, 5.0f, -25.0f));

	meshes["torus"].get_transform().translate(vec3(0.0f, 10.0f, 0.0f));

	//Set mesh material values
	//Red box
	meshes["box"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["box"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["box"].get_material().set_shininess(25.0f);
	meshes["box"].get_material().set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));

	//Green tetrahedron
	meshes["tetra"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["tetra"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["tetra"].get_material().set_shininess(25.0f);
	meshes["tetra"].get_material().set_diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	
	// Blue pyramid
	meshes["pyramid"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["pyramid"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["pyramid"].get_material().set_shininess(25.0f);
	meshes["pyramid"].get_material().set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	
	// White disk
	meshes["disk"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["disk"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["disk"].get_material().set_shininess(25.0f);
	meshes["disk"].get_material().set_diffuse(vec4(1.0f, 1.0f, 0.0f, 1.0f));

	// White torus
	meshes["torus"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["torus"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["torus"].get_material().set_shininess(15.0f);
	meshes["torus"].get_material().set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Brown cylinder for normal map
	normal_meshes["cylinder"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	normal_meshes["cylinder"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	normal_meshes["cylinder"].get_material().set_shininess(25.0f);
	normal_meshes["cylinder"].get_material().set_diffuse(vec4(0.53f, 0.45f, 0.37f, 1.0f));

	//Brown sphere for normal map
	normal_meshes["nSphere"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	normal_meshes["nSphere"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	normal_meshes["nSphere"].get_material().set_shininess(25.0f);
	normal_meshes["nSphere"].get_material().set_diffuse(vec4(0.53f, 0.45f, 0.37f, 1.0f));

	//Set hierarchy material details
	//White sphere with red emissive
	h_meshes[0].get_material().set_emissive(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	h_meshes[0].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	h_meshes[0].get_material().set_shininess(25.0f);
	h_meshes[0].get_material().set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//White sphere with green emissive
	h_meshes[1].get_material().set_emissive(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	h_meshes[1].get_material().set_specular(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	h_meshes[1].get_material().set_shininess(25.0f);
	h_meshes[1].get_material().set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//White sphere with blue emissive
	h_meshes[2].get_material().set_emissive(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	h_meshes[2].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	h_meshes[2].get_material().set_shininess(25.0f);
	h_meshes[2].get_material().set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//Create a mesh of the teapot object for shadowing
	meshes["teapot"] = mesh(geometry("res/models/teapot.obj"));
	// Translate Teapot
	meshes["teapot"].get_transform().translate(vec3(40.0f, 4.0f, -30.0f));
	// Scale the teapot - (0.1, 0.1, 0.1)
	meshes["teapot"].get_transform().scale = vec3(0.1f, 0.1f, 0.1f);

	//Set teapot material details - Red
	meshes["teapot"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["teapot"].get_material().set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	meshes["teapot"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["teapot"].get_material().set_shininess(15.0f);
   
	//Set textures
	tex = texture("res/textures/brick.jpg");

	//Transform hierarchy textures
	textures[0] = texture("res/textures/brick.jpg");
	textures[1] = texture("res/textures/grass.jpg");
	textures[2] = texture("res/textures/snow.jpg");

	//normal map texture
	normal_map = texture("res/textures/brick_normalmap.jpg");

	//Set values of point lights
	//red
	points[0].set_position(vec3(0.0f, 17.0f, 0.0f));
	points[0].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	points[0].set_range(50.0f);
	
	//blue
	points[1].set_position(vec3(0.0f, 8.5f, -14.0f));
	points[1].set_light_colour(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	points[1].set_range(20.0f);

	//green
	points[2].set_position(vec3(0.0f, 13.0f, 14.0f));
	points[2].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	points[2].set_range(20.0f);

	//red
	points[3].set_position(vec3(-10.0f, 13.0f, -35.0f));
	points[3].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	points[3].set_range(40.0f);

	//Set values of spot lights - both white
	spots[0].set_position(vec3(0.0f, 40.0f, 10.0f));
	spots[0].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[0].set_direction(normalize(vec3(0.0f, -1.0f, -1.0f)));
	spots[0].set_range(100.0f);
	spots[0].set_power(0.2f);

	spots[1].set_position(vec3(40.0f, 20.0f, -10.0f));
	spots[1].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[1].set_direction(normalize(vec3(0.0f, -1.0f, -1.0f)));
	spots[1].set_range(50.0f);
	spots[1].set_power(0.5f);

	//white ambient light
	d_light.set_ambient_intensity(vec4(0.1f, 0.1f, 0.1f, 1.0f));
	d_light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	d_light.set_direction(normalize(vec3(1.0f, 1.0f, 0.0f)));

  // Load in shaders for main effect
  eff.add_shader("res/shaders/shader.vert", GL_VERTEX_SHADER);
	  vector<string> frag_shaders{ "res/shaders/shader.frag", "res/shaders/point.frag",
		  "res/shaders/spot.frag", "res/shaders/shadow.frag", "res/shaders/normal_map.frag", "res/shaders/direction.frag" };
   eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

  // Load in shaders for shadow effect
  shadow_eff.add_shader("res/shaders/shader.vert", GL_VERTEX_SHADER);
  shadow_eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

  // Load in shaders for normal map effect
   normal_map_eff.add_shader("res/shaders/shader.vert", GL_VERTEX_SHADER);
   normal_map_eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

  // Build effects
  eff.build();
  shadow_eff.build();
  normal_map_eff.build();

  // Set camera properties - normal, free, arc and chase cams
  cam.set_position(vec3(100.0f, 10.0f, 100.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

  free_cam.set_position(vec3(50.0f, 10.0f, 50.0f));
  free_cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  free_cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

  arc_cam.set_distance(100.0f);
  arc_cam.set_target(h_meshes[0].get_transform().position);
  arc_cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

  chase_cam.set_pos_offset(vec3(10.0f, 25.0f, 10.0f));
  chase_cam.set_springiness(0.5f);
  //move the chase camera to where the box is moving
  chase_cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

  return true;
}

bool update(float delta_time) {
	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height =
		(quarter_pi<float>() *
		(static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) /
		static_cast<float>(renderer::get_screen_height());

	double current_x;
	double current_y;
	// *********************************
	// Get the current cursor position
	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);
	// Calculate delta of cursor positions from last frame
	double delta_x = current_x - cursor_x;
	double delta_y = current_y - cursor_y;
	// Multiply deltas by ratios - gets actual change in orientation
	delta_x = delta_x * ratio_width;
	delta_y = delta_y * ratio_height * -1;
	// Rotate cameras by delta
	free_cam.rotate(delta_x, delta_y);
	// delta_y - x-axis rotation
	delta_y - free_cam.get_pitch();
	// delta_x - y-axis rotation
	delta_x - free_cam.get_yaw();
	// Use keyboard to move the camera - WSAD
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W)) {
		free_cam.move(vec3(0.0f, 0.0f, 1.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S)) {
		free_cam.move(vec3(0.0f, 0.0f, -1.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A)) {
		free_cam.move(vec3(-1.0f, 0.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D)) {
		free_cam.move(vec3(1.0f, 0.0f, 0.0f));
	}

	// Use keyboard to move the shadow spotlight - UJHK : Y = UP and I = down
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_U)) {
		spots[1].move(vec3(0.0f, 0.0f, 1.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_J)) {
		spots[1].move(vec3(0.0f, 0.0f, -1.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_H)) {
		spots[1].move(vec3(1.0f, 0.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_K)) {
		spots[1].move(vec3(-1.0f, 0.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_Y)) {
		spots[1].move(vec3(0.0f, 1.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_I)) {
		spots[1].move(vec3(0.0f, -1.0f, 0.0f));
	}

	//If "F" pressed then remove ambient light that shows normal map on cylinder and sphere next to it
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F))
	{
		d_light.set_light_colour(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	//If "G" pressed then add ambient light that shows normal map on cylinder and sphere next to it
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_G))
	{
		d_light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//When "3" is pressed the arc cam rotates - does not rotate when not being viewed
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_3)) {
		arc_cam.rotate(0.0, 0.01);
	}

	//Move the box mesh for the chase cam
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
		meshes["box"].get_transform().translate(vec3(0.0f, 0.0f, -0.2f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
		meshes["box"].get_transform().translate(vec3(0.0f, 0.0f, 0.2f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT)) {
		meshes["box"].get_transform().translate(vec3(-0.2f, 0.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT)) {
		meshes["box"].get_transform().translate(vec3(0.2f, 0.0f, 0.0f));
	}

	//move the chase camera to where the box is moving
	chase_cam.move(meshes["box"].get_transform().position, eulerAngles(meshes["box"].get_transform().orientation));

	//Rotate the transform hierarchy objects in relation to the parent - two spheres spinning around the main one
	h_meshes[0].get_transform().rotate(vec3(0.0f, delta_time, 0.0f));
	h_meshes[1].get_transform().rotate(vec3(0.0f, 0.0f, delta_time));

	//Rotate the torus
	meshes["torus"].get_transform().rotate(vec3(delta_time, 0.0f, 0.0f));

	// Update the cameras
	free_cam.update(delta_time);
	cam.update(delta_time);
	arc_cam.update(delta_time);
	chase_cam.update(delta_time);

	//Sets the shadow to the spotlight projecting on to the teapot
	shadow.light_position = spots[1].get_position();
	shadow.light_dir = spots[1].get_direction();

	// Update cursor pos
	glfwSetCursorPos(renderer::get_window(), cursor_x, cursor_y);

   return true;
}

bool render() {
	//Render shadow map
	// Set render target to shadow map
	renderer::set_render_target(shadow);
	// Clear depth buffer bit
	glClear(GL_DEPTH_BUFFER_BIT);
	// Set face cull mode to front
	glCullFace(GL_FRONT);

	//Ensure not clipping with shadowing
	mat4 LightProjectionMat = perspective<float>(90.f, renderer::get_screen_aspect(), 0.1f, 1000.f);

	// Bind shadow effect
	renderer::bind(shadow_eff);

	//Sets m to the teapot mesh for shadowing
	auto m = meshes["teapot"];
	// Create MVP matrix
	auto M = m.get_transform().get_transform_matrix();
	// *********************************
	// View matrix taken from shadow map
	auto V = shadow.get_view();
	// *********************************
	auto MVP = LightProjectionMat * V * M;
	// Set MVP matrix uniform
	glUniformMatrix4fv(shadow_eff.get_uniform_location("MVP"), // Location of uniform
		1,                                      // Number of values - 1 mat4
		GL_FALSE,                               // Transpose the matrix?
		value_ptr(MVP));                        // Pointer to matrix data
												// Render mesh
	renderer::render(m);

	// *********************************
	// Set render target back to the screen
	renderer::set_render_target();
	// Set face cull mode to back
	glCullFace(GL_BACK);


	// Bind main effect
	renderer::bind(eff);

	//Render all other meshes
	for (auto &e : meshes)
	{
		auto m = e.second;
		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix for default view
		auto M = m.get_transform().get_transform_matrix();
		auto V = cam.get_view();
		auto P = cam.get_projection();

		//While "2" is held then set view to free_cam
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_2) == GLFW_PRESS) {
			V = free_cam.get_view();
			P = free_cam.get_projection();
		}

		//While "3" is held then set view to arc_cam
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_3) == GLFW_PRESS) {
			V = arc_cam.get_view();
			P = arc_cam.get_projection();
		}

		//While "4" is held then set view to chase_cam
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_4) == GLFW_PRESS) {
			V = chase_cam.get_view();
			P = chase_cam.get_projection();
		}

		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));

		//Gets required view details for lightMVP Matrix uniform - used for shadowing
		auto viewM = shadow.get_view();
		auto lightMVP = LightProjectionMat * viewM * M;
		glUniformMatrix4fv(eff.get_uniform_location("lightMVP"), 1, GL_FALSE, value_ptr(lightMVP));

		//Bind materials, lights and textures
		renderer::bind(m.get_material(), "mat");
		renderer::bind(points, "points");
		renderer::bind(spots, "spots");
		renderer::bind(tex, 0);

		//Set texture uniform and eye_pos uniform
		glUniform1i(eff.get_uniform_location("tex"), 0);
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));

		//Bind the shadow map and set it's uniform
		renderer::bind(shadow.buffer->get_depth(), 1);
		glUniform1i(eff.get_uniform_location("shadow_map"), 1);

		// Render geometry
		renderer::render(m);
	}

	//For each mesh in h_meshes array
	for (size_t i = 0; i < h_meshes.size(); i++) {
		// *********************************
		// SET M to be the usual mesh  transform matrix
		mat4 M = h_meshes[i].get_transform().get_transform_matrix();

		// Apply the heirarchy chain
		for (size_t j = i; j > 0; j--) {
			M = h_meshes[j - 1].get_transform().get_transform_matrix() * M;
		}

		//Get camera view and projection
		auto V = cam.get_view();
		auto P = cam.get_projection();

		//Code for switching cameras (same as above)
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_2) == GLFW_PRESS) {
			V = free_cam.get_view();
			P = free_cam.get_projection();
		}

		if (glfwGetKey(renderer::get_window(), GLFW_KEY_3) == GLFW_PRESS) {
			V = arc_cam.get_view();
			P = arc_cam.get_projection();
		}

		if (glfwGetKey(renderer::get_window(), GLFW_KEY_4) == GLFW_PRESS) {
			V = chase_cam.get_view();
			P = chase_cam.get_projection();
		}

		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	 	glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(h_meshes[i].get_transform().get_normal_matrix()));

		//bind texture array
		renderer::bind(textures[i], 0);

		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));

		// Render geometry
		renderer::render(h_meshes[i]);
	}

	//Render meshes for normal mapping
	for (auto &e : normal_meshes)
	{
		auto m = e.second;
		auto M = m.get_transform().get_transform_matrix();
	    auto V = cam.get_view();
		auto P = cam.get_projection();

		//Code for switching cameras (same as above)
	  	if (glfwGetKey(renderer::get_window(), GLFW_KEY_2) == GLFW_PRESS) {
			V = free_cam.get_view();
			P = free_cam.get_projection();
		}

		if (glfwGetKey(renderer::get_window(), GLFW_KEY_3) == GLFW_PRESS) {
			V = arc_cam.get_view();
			P = arc_cam.get_projection();
		}

		 if (glfwGetKey(renderer::get_window(), GLFW_KEY_4) == GLFW_PRESS) {
			V = chase_cam.get_view();
			P = chase_cam.get_projection();
		}

		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		// Set M matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		// Set N matrix uniform
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE,
			value_ptr(m.get_transform().get_normal_matrix()));
		// *********************************
		// Bind material
		renderer::bind(m.get_material(), "mat");

		// Bind lights
		renderer::bind(spots, "light");
		renderer::bind(points, "points");
	  	 renderer::bind(d_light, "d_light");

		// Bind normal_map
		renderer::bind(normal_map, 1);
		// Set normal_map uniform
		glUniform1i(eff.get_uniform_location("normal_map"), 1);
		// Bind texture
		renderer::bind(tex, 0);
		// Set tex uniform
		glUniform1i(eff.get_uniform_location("tex"), 0);

		// Set eye position
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));

		// Render mesh
		renderer::render(m);
	}

	return true;
}

void main() {
  // Create application
  app application("Graphics Coursework");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_initialise(initialise);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}