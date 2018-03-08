#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
effect eff;
effect shadow_eff;
texture tex;
target_camera cam;
free_camera free_cam;
arc_ball_camera arc_cam;
chase_camera chase_cam;
vector<point_light> points(4);
vector<spot_light> spots(2);

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

	shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());

	meshes["plane"] = mesh(geometry_builder::create_plane());
	// Create scene
	meshes["box"] = mesh(geometry_builder::create_box());
	meshes["tetra"] = mesh(geometry_builder::create_tetrahedron());
	meshes["pyramid"] = mesh(geometry_builder::create_pyramid());
	meshes["disk"] = mesh(geometry_builder::create_disk(20));
	meshes["cylinder"] = mesh(geometry_builder::create_cylinder(20, 20));

	meshes["main"] = mesh(geometry_builder::create_pyramid());
	meshes["ring"] = mesh(geometry_builder::create_torus(20, 10, 1.0f,7.0f));
	meshes["planet1"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["planet2"] = mesh(geometry_builder::create_sphere(20, 20));

	meshes["main"].get_transform().translate(vec3(0.0f, 10.0f, 0.0f));
	meshes["main"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["ring"].get_transform().translate(vec3(0.0f, 10.0f, 0.0f));

	meshes["planet1"].get_transform().scale = vec3(2.5f, 2.5f, 2.5f);
	meshes["planet1"].get_transform().translate(vec3(0.0f, 10.0f, -14.0f));
	meshes["planet2"].get_transform().scale = vec3(2.0f, 2.0f, 2.0f);
	meshes["planet2"].get_transform().translate(vec3(0.0f, 10.0f, 14.0f));

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
	meshes["cylinder"].get_transform().scale = vec3(5.0f, 10.0f, 5.0f);
	meshes["cylinder"].get_transform().translate(vec3(0.0f, 2.5f, 0.0f));


	meshes["box"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["box"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["box"].get_material().set_shininess(25.0f);
	meshes["box"].get_material().set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	// Green tetra

	meshes["tetra"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["tetra"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["tetra"].get_material().set_shininess(25.0f);
	meshes["tetra"].get_material().set_diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	// Blue pyramid

	meshes["pyramid"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["pyramid"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["pyramid"].get_material().set_shininess(25.0f);
	meshes["pyramid"].get_material().set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	// Yellow disk

	meshes["disk"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["disk"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["disk"].get_material().set_shininess(25.0f);
	meshes["disk"].get_material().set_diffuse(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	// Magenta cylinder

	meshes["cylinder"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["cylinder"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["cylinder"].get_material().set_shininess(25.0f);
	meshes["cylinder"].get_material().set_diffuse(vec4(1.0f, 0.0f, 1.0f, 1.0f));
	// Cyan sphere

	meshes["main"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["main"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["main"].get_material().set_shininess(25.0f);
	meshes["main"].get_material().set_diffuse(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	// White torus

	meshes["ring"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["ring"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["ring"].get_material().set_shininess(25.0f);
	meshes["ring"].get_material().set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));


	meshes["teapot"] = mesh(geometry("res/models/teapot.obj"));
	// Translate Teapot(0,4,0)
	meshes["teapot"].get_transform().translate(vec3(40.0f, 4.0f, -30.0f));
	// Scale the teapot - (0.1, 0.1, 0.1)
	meshes["teapot"].get_transform().scale = vec3(0.1f, 0.1f, 0.1f);
	meshes["teapot"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["teapot"].get_material().set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	meshes["teapot"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["teapot"].get_material().set_shininess(25.0f);
   
	tex = texture("res/textures/brick.jpg");

	points[0].set_position(vec3(0.0f, 17.0f, 0.0f));
	points[0].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	points[0].set_range(50.0f);
	
	points[1].set_position(vec3(0.0f, 8.5f, -14.0f));
	points[1].set_light_colour(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	points[1].set_range(20.0f);

	points[2].set_position(vec3(0.0f, 13.0f, 14.0f));
	points[2].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	points[2].set_range(20.0f);

	points[3].set_position(vec3(-10.0f, 13.0f, -35.0f));
	points[3].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	points[3].set_range(20.0f);

	spots[0].set_position(vec3(0.0f, 40.0f, 0.0f));
	spots[0].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[0].set_direction(vec3(0.0f, -1.0f, 0.0f));
	spots[0].set_range(50.0f);
	spots[0].set_power(1.0f);

	spots[1].set_position(vec3(40.0f, 20.0f, -10.0f));
	spots[1].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[1].set_direction(vec3(0.0f, -1.0f, -1.0f));
	spots[1].set_range(30.0f);
	spots[1].set_power(10.0f);

  // Load in shaders
    eff.add_shader("res/shaders/shader.vert", GL_VERTEX_SHADER);
  vector<string> frag_shaders{ "res/shaders/shader.frag", "res/shaders/direction.frag",
	    "res/shaders/point.frag", "res/shaders/spot.frag", "res/shaders/shadow.frag" };
  eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

  shadow_eff.add_shader("res/shaders/shader.vert", GL_VERTEX_SHADER);
  shadow_eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

  // Build effect
  eff.build();
  shadow_eff.build();

  // Set camera properties

  cam.set_position(vec3(100.0f, 10.0f, 100.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

  free_cam.set_position(vec3(50.0f, 10.0f, 50.0f));
  free_cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  free_cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

  arc_cam.set_distance(100.0f);
  arc_cam.set_target(meshes["main"].get_transform().position);
  arc_cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

  chase_cam.set_pos_offset(vec3(10.0f, 25.0f, 10.0f));
  chase_cam.set_springiness(0.5f);
  chase_cam.move(meshes["box"].get_transform().position, eulerAngles(meshes["box"].get_transform().orientation));
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

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_3)) {
		arc_cam.rotate(0.0, 0.01);
	}

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
	chase_cam.move(meshes["box"].get_transform().position, eulerAngles(meshes["box"].get_transform().orientation));

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
	// *********************************
	// Set render target to shadow map
	renderer::set_render_target(shadow);
	// Clear depth buffer bit
	glClear(GL_DEPTH_BUFFER_BIT);
	// Set face cull mode to front
	glCullFace(GL_FRONT);
	// *********************************

	// We could just use the Camera's projection, 
	// but that has a narrower FoV than the cone of the spot light, so we would get clipping.
	// so we have to create a new Proj Mat with a field of view of 90.
	mat4 LightProjectionMat = perspective<float>(90.f, renderer::get_screen_aspect(), 0.1f, 1000.f);

	// Bind shader
	renderer::bind(shadow_eff);
	// Render meshes

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


	// ***
	// Bind shader
	renderer::bind(eff);

	for (auto &e : meshes)
	{
		auto m = e.second;
		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = cam.get_view();
		auto P = cam.get_projection();

		if (glfwGetKey(renderer::get_window(), GLFW_KEY_1) == GLFW_PRESS) {
			cam.set_position(spots[1].get_position());
		}

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
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
		auto viewM = shadow.get_view();
		auto lightMVP = LightProjectionMat * viewM * M;
	    	glUniformMatrix4fv(eff.get_uniform_location("lightMVP"), 1, GL_FALSE, value_ptr(lightMVP));

		renderer::bind(m.get_material(), "mat");
		renderer::bind(points, "points");
		renderer::bind(spots, "spots");
		renderer::bind(tex, 0);
		glUniform1i(eff.get_uniform_location("tex"), 0);
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
		
		renderer::bind(shadow.buffer->get_depth(), 1);
		glUniform1i(eff.get_uniform_location("shadow_map"), 1);

		// Render geometry
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