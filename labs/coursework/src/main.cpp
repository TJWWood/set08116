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
vector<spot_light> spots(5);

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
	meshes["plane"] = mesh(geometry_builder::create_plane());
	// Create scene
	meshes["box"] = mesh(geometry_builder::create_box());
	meshes["tetra"] = mesh(geometry_builder::create_tetrahedron());
	meshes["pyramid"] = mesh(geometry_builder::create_pyramid());
	meshes["disk"] = mesh(geometry_builder::create_disk(20));
	meshes["cylinder"] = mesh(geometry_builder::create_cylinder(20, 20));
	meshes["sphere"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["torus"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 5.0f));

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
	meshes["cylinder"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["cylinder"].get_transform().translate(vec3(-25.0f, 2.5f, -25.0f));
	meshes["sphere"].get_transform().scale = vec3(2.5f, 2.5f, 2.5f);
	meshes["sphere"].get_transform().translate(vec3(-25.0f, 10.0f, -25.0f));
	meshes["torus"].get_transform().translate(vec3(-25.0f, 10.0f, -25.0f));
	meshes["torus"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));


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

	meshes["sphere"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["sphere"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["sphere"].get_material().set_shininess(25.0f);
	meshes["sphere"].get_material().set_diffuse(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	// White torus

	meshes["torus"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["torus"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["torus"].get_material().set_shininess(25.0f);
	meshes["torus"].get_material().set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));


	meshes["teapot"] = mesh(geometry_builder::create_box());
	// Translate Teapot(0,4,0)
	meshes["teapot"].get_transform().translate(vec3(0.0f, 4.0f, 0.0f));
	// Scale the teapot - (0.1, 0.1, 0.1)
	meshes["teapot"].get_transform().scale = vec3(0.1f, 0.1f, 0.1f);
	meshes["teapot"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["teapot"].get_material().set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	meshes["teapot"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["teapot"].get_material().set_shininess(25.0f);

	tex = texture("res/textures/checker.png");

	points[0].set_position(vec3(-25.0f, 5.0f, -15.0f));
	points[0].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	points[0].set_range(20.0f);
	// Point 1, Position (-25, 5, -35)
	// Red,20 range
	points[1].set_position(vec3(-25.0f, 5.0f, -35.0f));
	points[1].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	points[1].set_range(20.0f);
	// Point 2,Position (-10, 5, -15)
	// Red,20 range
	points[2].set_position(vec3(-10.0f, 5.0f, -15.0f));
	points[2].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	points[2].set_range(20.0f);
	// Point 3,Position (-10, 5, -35)
	// Red,20 range
	points[3].set_position(vec3(-10.0f, 5.0f, -35.0f));
	points[3].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	points[3].set_range(20.0f);
	// Spot 0, Position (-25, 10, -15)
	// Green, Direction (1, -1, -1) normalized
	// 20 range,0.5 power
	spots[0].set_position(vec3(-25.0f, 10.0f, -15.0f));
	spots[0].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	spots[0].set_direction(normalize(vec3(1.0f, -1.0f, -1.0f)));
	spots[0].set_range(20.0f);
	spots[0].set_power(0.5f);
	// Spot 1,Position (-25, 10, -35)
	// Green,Direction (1, -1, 1) normalized
	// 20 range,0.5 power
	spots[1].set_position(vec3(-25.0f, 10.0f, -35.0f));
	spots[1].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	spots[1].set_direction(normalize(vec3(1.0f, -1.0f, 1.0f)));
	spots[1].set_range(20.0f);
	spots[1].set_power(0.5f);
	// Spot 2,Position (-10, 10, -15)
	// Green,Direction (-1, -1, -1) normalized
	// 20 range,0.5 power
	spots[2].set_position(vec3(-10.0f, 10.0f, -15.0f));
	spots[2].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	spots[2].set_direction(normalize(vec3(-1.0f, -1.0f, -1.0f)));
	spots[2].set_range(20.0f);
	spots[2].set_power(0.5f);
	// Spot 3,Position (-10, 10, -35)
	// Green,Direction (-1, -1, 1) normalized
	// 20 range,0.5 power
	spots[3].set_position(vec3(-10.0f, 10.0f, -35.0f));
	spots[3].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	spots[3].set_direction(normalize(vec3(-1.0f, -1.0f, 1.0f)));
	spots[3].set_range(20.0f);
	spots[3].set_power(0.5f);
	// Spot 4,Position (-17.5, 15, -25)
	// Blue,Direction (0, -1, 0)
	// 30 range,1.0 power
	spots[4].set_position(vec3(-17.5f, 15.0f, -25.0f));
	spots[4].set_light_colour(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	spots[4].set_direction(vec3(0.0f, -1.0f, 0.0f));
	spots[4].set_range(30.0f);
	spots[4].set_power(1.0f);

  // Load in shaders
    eff.add_shader("res/shaders/shader.vert", GL_VERTEX_SHADER);
  vector<string> frag_shaders{ "res/shaders/shader.frag", "res/shaders/direction.frag",
	    "res/shaders/point.frag", "res/shaders/spot.frag" };
  eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

  // Build effect
  eff.build();

  // Set camera properties

  cam.set_position(vec3(100.0f, 10.0f, 100.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

  free_cam.set_position(vec3(50.0f, 10.0f, 50.0f));
  free_cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  free_cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

  arc_cam.set_distance(100.0f);
  arc_cam.set_target(meshes["sphere"].get_transform().position);
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

	// Update the camera
	free_cam.update(delta_time);
	cam.update(delta_time);
	arc_cam.update(delta_time);
	chase_cam.update(delta_time);

	// Update cursor pos
	glfwSetCursorPos(renderer::get_window(), cursor_x, cursor_y);

   return true;
}

bool render() {

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
			V = cam.get_view();
			P = cam.get_projection();
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

		renderer::bind(m.get_material(), "mat");
		renderer::bind(points, "points");
		renderer::bind(spots, "spots");
		renderer::bind(tex, 0);
		glUniform1i(eff.get_uniform_location("tex"), 0);
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
		
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