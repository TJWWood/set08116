#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace std::chrono;
using namespace graphics_framework;
using namespace glm;

const unsigned int MAX_PARTICLES = 50000;

vec4 positions[MAX_PARTICLES];
vec4 velocitys[MAX_PARTICLES];

GLuint G_Position_buffer, G_Velocity_buffer;
effect smoke_eff;
effect compute_eff;
GLuint vao;
texture smoke_tex;

map<string, mesh> meshes;
effect eff;
effect shadow_eff;
effect explode_eff;
effect tex_eff;

texture tex;
free_camera cam;
vector<point_light> points(4);
vector<spot_light> spots(1);

frame_buffer frame;
geometry screen_quad;
texture red_flash;

float explode_factor = 0.0f;
string currentMesh = "";
int playerHP = 100;
bool playerHit = false;
vec3 tempDirection;

double cursor_x = 0.0f;
double cursor_y = 0.0f;

shadow_map shadow;

bool initialise()
{
	glfwSetCursorPos(renderer::get_window(), renderer::get_screen_width() / 2 , renderer::get_screen_height() / 2);
	return true;
}

bool load_content() {

	//random distribution and generation and movement of smoke particles
	default_random_engine rand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()); 
	uniform_real_distribution<float> dist;
	smoke_tex = texture("res/textures/smoke.png");
	           
	for (unsigned int i = 0; i < MAX_PARTICLES; ++i) {
		positions[i] = vec4(((2.0f * dist(rand)) - 1.0f) / 2.0f, 2.0 * dist(rand), 0.0f, 0.0f);
		velocitys[i] = vec4(0.2f * dist(rand), 0.2f * dist(rand), 0.2f * dist(rand), 0.2f * dist(rand)); 
	}

	// Load in shaders
	compute_eff.add_shader("res/shaders/particle.comp", GL_COMPUTE_SHADER);
	compute_eff.build();

	// a useless vao, but we need it bound or we get errors.
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// *********************************
	//Generate Position Data buffer
	glGenBuffers(4, &G_Position_buffer);
	// Bind as GL_SHADER_STORAGE_BUFFER
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, G_Position_buffer);
	// Send Data to GPU, use GL_DYNAMIC_DRAW
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(G_Position_buffer) * MAX_PARTICLES, positions, GL_DYNAMIC_DRAW);

	// Generate Velocity Data buffer
	glGenBuffers(5, &G_Velocity_buffer);
	// Bind as GL_SHADER_STORAGE_BUFFER
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, G_Velocity_buffer);
	// Send Data to GPU, use GL_DYNAMIC_DRAW
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(G_Velocity_buffer) * MAX_PARTICLES, velocitys, GL_DYNAMIC_DRAW);
	// *********************************  
	//Unbind
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);



	// Create frame buffer - use screen width and height
	frame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	// Create screen quad
	vector<vec3> positions{ vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f),
		vec3(1.0f, 1.0f, 0.0f) };
	vector<vec2> tex_coords{ vec2(0.0, 0.0), vec2(1.0f, 0.0f), vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f) };  
	// *********************************
	screen_quad.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	screen_quad.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	screen_quad.set_type(GL_TRIANGLE_STRIP);

	//Initialise a new shadow map
	shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());

	// Create scene
	meshes["plane"] = mesh(geometry_builder::create_plane());
	meshes["enemy1"] = mesh(geometry_builder::create_sphere(40, 40));
	meshes["enemy2"] = mesh(geometry_builder::create_sphere(40, 40));
	meshes["enemy3"] = mesh(geometry_builder::create_sphere(40, 40));
	meshes["enemy4"] = mesh(geometry_builder::create_sphere(40, 40));
	meshes["projectile"] = mesh(geometry_builder::create_sphere(40,40));

	// Transform objects

	meshes["enemy1"].get_transform().scale = vec3(1.0f, 1.0f, 1.0f);
	meshes["enemy1"].get_transform().translate(vec3(40.0f, 8.0f, 0.0f));

	meshes["enemy2"].get_transform().scale = vec3(1.0f, 1.0f, 1.0f);
	meshes["enemy2"].get_transform().translate(vec3(40.0f, 8.0f, -20.0f));

	meshes["enemy3"].get_transform().scale = vec3(1.0f, 1.0f, 1.0f);
	meshes["enemy3"].get_transform().translate(vec3(40.0f, 8.0f, -40.0f));

	meshes["enemy4"].get_transform().scale = vec3(1.0f, 1.0f, 1.0f);
	meshes["enemy4"].get_transform().translate(vec3(40.0f, 8.0f, 20.0f));

	meshes["projectile"].get_transform().scale = vec3(0.2f,0.2f,0.2f);

	//Set mesh material values
	meshes["enemy1"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["enemy1"].get_material().set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	meshes["enemy1"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["enemy1"].get_material().set_shininess(15.0f);

	meshes["enemy2"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["enemy2"].get_material().set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	meshes["enemy2"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["enemy2"].get_material().set_shininess(15.0f);

	meshes["enemy3"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["enemy3"].get_material().set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	meshes["enemy3"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["enemy3"].get_material().set_shininess(15.0f);

	meshes["enemy4"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["enemy4"].get_material().set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	meshes["enemy4"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["enemy4"].get_material().set_shininess(15.0f);

	//Set textures
	tex = texture("res/textures/brick.jpg");
	red_flash = texture("res/textures/red_flash.png");

	//Set values of point lights
	//red
	points[0].set_position(vec3(40.0f, 4.0f, 0.0f));
	points[0].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	points[0].set_range(50.0f);

	//blue
	points[1].set_position(vec3(40.0f, 4.0f, -20.0f));
	points[1].set_light_colour(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	points[1].set_range(20.0f);

	//green
	points[2].set_position(vec3(40.0f, 4.0f, -40.0f));
	points[2].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	points[2].set_range(20.0f);

	//red
	points[3].set_position(vec3(40.0f, 4.0f, 20.0f));
	points[3].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	points[3].set_range(40.0f);

	//Set values of spot lights - both white
	spots[0].set_position(vec3(0.0f, 40.0f, 20.0f));
	spots[0].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[0].set_direction(normalize(vec3(0.0f, -1.0f, -1.0f)));
	spots[0].set_range(100.0f);
	spots[0].set_power(0.1f);

	// Load in shaders for main effect
	eff.add_shader("res/shaders/shader.vert", GL_VERTEX_SHADER);
	vector<string> frag_shaders{ "res/shaders/shader.frag", "res/shaders/point.frag",
		"res/shaders/spot.frag", "res/shaders/shadow.frag" };
	eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

	explode_eff.add_shader("res/shaders/explode_vert.vert", GL_VERTEX_SHADER);
	explode_eff.add_shader("res/shaders/explode_frag.frag", GL_FRAGMENT_SHADER);
	explode_eff.add_shader("res/shaders/explode.geom", GL_GEOMETRY_SHADER); 

	// Load in shaders for shadow effect
	shadow_eff.add_shader("res/shaders/shader.vert", GL_VERTEX_SHADER);
	shadow_eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

	//load shaders for damage flash
	tex_eff.add_shader("res/shaders/simple_texture.vert", GL_VERTEX_SHADER);
	tex_eff.add_shader("res/shaders/redflash.frag", GL_FRAGMENT_SHADER);

	smoke_eff.add_shader("res/shaders/smoke.vert", GL_VERTEX_SHADER);
	smoke_eff.add_shader("res/shaders/smoke.frag", GL_FRAGMENT_SHADER);
	smoke_eff.add_shader("res/shaders/smoke.geom", GL_GEOMETRY_SHADER);
	
	// Build effects   
	eff.build();
	explode_eff.build(); 
	shadow_eff.build();
	tex_eff.build();
	smoke_eff.build();

	// Set free camera properties
	cam.set_position(vec3(0.0f, 8.0f, 0.0f));
	cam.set_target(vec3(90.0f, 0.0f, 0.0f));
	cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

	//default projectile pos
	meshes["projectile"].get_transform().translate(vec3(0.0f, 8.0f, 0.0f));


	return true;
}

bool update(float delta_time) {
	if (delta_time > 10.0f) {
		delta_time = 10.0f;
	}

	//set uniforms for compute eff for smoke eff
	renderer::bind(compute_eff);
	glUniform3fv(compute_eff.get_uniform_location("max_dims"), 1, &(vec3(3.0f, 5.0f, 5.0f))[0]);
	glUniform1f(compute_eff.get_uniform_location("delta_time"), delta_time);

	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height =
		(quarter_pi<float>() *
		(static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) /
		static_cast<float>(renderer::get_screen_height());

	// Update cursor pos
	double current_x;
	double current_y;
	// *********************************
	// Get the current cursor position
	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D)) {
		cam.rotate(0.1f, 0.0f);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A)) {
		cam.rotate(-0.1f, 0.0f);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W)) {
		cam.move(vec3(0.0f, 0.0f, 1.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S)) {
		cam.move(vec3(0.0f, 0.0f, -1.0f));
	}

	//update camera
	cam.update(delta_time);

	//Sets the shadow to the main spotlight
	shadow.light_position = spots[0].get_position();
	shadow.light_dir = spots[0].get_direction();

	double mouse_X;
	double mouse_Y;

	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);

	// Origin and direction of the ray
	glm::vec3 origin;
	glm::vec3 direction;
	float distance = 0.0f;
	// Convert mouse position to ray
	screen_pos_to_world_ray(cursor_x, cursor_y, renderer::get_screen_width(), renderer::get_screen_height(), cam.get_view(), cam.get_projection(), origin, direction);

	// *********************************
	// Check all the meshes for intersection
	for (auto &m : meshes) {
		//if detected mesh is plane then ensure it doesn't move
		if (m.first == "plane")
		{
			m.second.get_transform().position = vec3(0.0f, 0.0f, 0.0f); 
		}
		//if detected mesh is the projectile then move it in the direction of the raycast
		else if (m.first == "projectile")
		{
			meshes["projectile"].get_transform().position += tempDirection * delta_time * 100.0f;
		}
		//if detected mesh is one of the enemies then ensure they don't endlessly fly
		else if (m.first == "enemy1" || m.first == "enemy2" || m.first == "enemy3" || m.first == "enemy4")
		{
			if (m.second.get_transform().position.x <= -40.0f)
			{
				m.second.get_transform().position.x = 40.0f;
			}
		}
		//move enemies along x-axis
		m.second.get_transform().position -= vec3(0.1f, 0.0f, 0.0f);

		//if enemy is red then stop it moving
		if (m.second.get_material().get_diffuse() == vec4(1.0f, 0.0f, 0.0f, 1.0f))
		{
			m.second.get_transform().position += vec3(0.1f, 0.0f, 0.0f);
		}

		//Game Logic Below - Press space to fire at the enemies
		if (glfwGetKey(renderer::get_window(), GLFW_KEY_SPACE))
		{
			tempDirection = direction;
			meshes["projectile"].get_transform().position = cam.get_position();
			//check if ray hits anything
			if (test_ray_oobb(origin, direction, m.second.get_minimal(), m.second.get_maximal(),
				m.second.get_transform().get_transform_matrix(), distance))
			{
				explode_factor = 30.0f;
				cout << distance;
				//if distance is less than or equal to 10 units then hurt the player and reset enemy position
				if (distance <= 10.0f)
				{
					explode_factor = 70.0f;
					//make sure the projectile doesn't count as an enemy (it's default position is where the camera view is so can get caught as false positive
					if (m.first == "projectile")
					{	
						cout << "Nothing done";
					}
					else
					{
						playerHit = true;
						playerHP -= 10;
						m.second.get_transform().position.x = 40.0f;
						cout << "too close, hurts player and resets";
					}

				}
				//if more than 10 units away then hurt the enemy
				else if(distance > 10.0f)
				{
					playerHit = false;

					cout << "Safe distance - Enemy hurt";

					//Code does not work fully due to raycast affecting the mesh multiple times on pressing space
					//Meant to change mesh colour to indicate hurting it, then when it is red it is destroyed
					if (m.second.get_material().get_diffuse().z == 1.0f)
					{
						m.second.get_material().set_diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f));
					}
					if (m.second.get_material().get_diffuse().y == 1.0f)
					{
						m.second.get_material().set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
					}
					//sets detected mesh as global var for use in render()
					currentMesh = m.first;
				}
			}
		}	
		//causes moving explosion effect
		explode_factor -= 0.7f;
	}

	//checks for player death
	if (playerHP == 0)
	{
		cout << "YOU ARE DEAD";
	}
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

	//Sets m to the mesh for shadowing
	auto m = meshes["enemy1"];
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

	renderer::bind(compute_eff);
	// Bind data as SSBO
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, G_Position_buffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, G_Velocity_buffer);
	// Dispatch
	glDispatchCompute(MAX_PARTICLES / 128, 1, 1);
	// Sync, wait for completion
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	renderer::bind(eff);

	renderer::set_render_target(frame);
	// Clear frame
	renderer::clear();
	// Set render target back to the screen
	renderer::set_render_target();

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


		renderer::bind(eff);
		// Render geometry

		if (e.first == currentMesh)
		{
			renderer::bind(explode_eff);
			glUniformMatrix4fv(explode_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
			glUniform1f(explode_eff.get_uniform_location("explode_factor"), explode_factor);
			renderer::render(m);
		}
		renderer::bind(eff);
		renderer::render(m);
	}

	renderer::bind(tex_eff);

	//// *********************************
	if (playerHit == true)
	{
		// Set render target back to the screen
		renderer::set_render_target();
		// Bind Tex effect
		renderer::bind(tex_eff);
		// MVP is now the identity matrix
		M = mat4(1.0);
		// Set MVP matrix uniform
		glUniformMatrix4fv(tex_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(M));
		// Bind texture from frame buffer to TU 0
		renderer::bind(red_flash, 0);
		// Set the tex uniform, 0
		glUniform1i(tex_eff.get_uniform_location("tex"), 0);
		// Render the screen quad
		renderer::render(screen_quad);
		playerHit = false;
	}

	for (auto &q : meshes)
	{
		if (q.first == currentMesh)
		{
			renderer::bind(smoke_eff);
			m = q.second;
			// Create MV matrix
			M = q.second.get_transform().get_transform_matrix();
			V = cam.get_view();
			auto MV = V * M;
			auto P = cam.get_projection();
			// Set the colour uniform
			glUniform4fv(smoke_eff.get_uniform_location("colour"), 1, value_ptr(vec4(1.0f, 0.0f, 0.0f, 1.0f)));
			// Set MV, and P matrix uniforms seperatly
			glUniformMatrix4fv(
				smoke_eff.get_uniform_location("MV"),
				1,
				GL_FALSE,
				value_ptr(MV));
			glUniformMatrix4fv(smoke_eff.get_uniform_location("P"), 1, GL_FALSE, value_ptr(P));
			// Set point_size size uniform to .1f
			glUniform1f(smoke_eff.get_uniform_location("point_size"), 0.1f);
			// Bind particle texture
			renderer::bind(smoke_tex, 4);
			glUniform1i(smoke_eff.get_uniform_location("tex"), 4);
			// *********************************

			// Bind position buffer as GL_ARRAY_BUFFER
			glBindBuffer(GL_ARRAY_BUFFER, G_Position_buffer);
			// Setup vertex format
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
			// Enable Blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			// Disable Depth Mask
			glDepthMask(GL_FALSE);
			// Render
			glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
			// Tidy up, enable depth mask
			glDepthMask(GL_TRUE);
			// Disable Blend
			glDisable(GL_BLEND);
			// Unbind all arrays
			glDisableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glUseProgram(0);

			renderer::bind(eff);
			renderer::render(m);
		}
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