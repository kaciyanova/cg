#include <glm\glm.hpp>
#include <graphics_framework.h>
#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
map<string, texture> textures;
effect eff;
effect shadow_eff;

free_camera cam;

shadow_map shadow;

spot_light spot;

double cursor_x = 0.0;
double cursor_y = 0.0;
GLFWwindow* window;


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

	// Create plane mesh
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
	meshes["disk"].get_transform().orientation = vec3(half_pi<float>(), 0.0f, 0.0f);

	meshes["cylinder"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["cylinder"].get_transform().translate(vec3(-25.0f, 2.5f, -25.0f));

	meshes["sphere"].get_transform().scale = vec3(2.5f, 2.5f, 2.5f);
	meshes["sphere"].get_transform().translate(vec3(-25.0f, 10.0f, -25.0f));

	meshes["torus"].get_transform().translate(vec3(-25.0f, 10.0f, -25.0f));
	meshes["torus"].get_transform().orientation = vec3(half_pi<float>(), 0.0f, 0.0f);

	// Create mesh to chase
	meshes["crow"] = mesh(geometry("models/Crow/CROW.OBJ"));
	meshes["crow"].get_transform().scale = vec3(15.0f);
	meshes["crow"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));

	// meshes["sword"] = mesh(geometry_builder::create_box());
	meshes["sword"] = mesh(geometry("models/claymore.obj"));
	meshes["sword"].get_transform().scale = vec3(0.05f);
	meshes["sword"].get_transform().translate(vec3(-0.5f, 0.21f, 0.12f));
	meshes["sword"].get_transform().orientation = vec3(half_pi<float>(), pi<float>(), half_pi<float>());

	// Load textures
	textures["plane"] = texture("textures/blackmarble.jpg");
	textures["box"] = texture("textures/checked.gif");
	textures["tetra"] = texture("textures/confirmed.png");
	textures["pyramid"] = texture("textures/sand.jpg");
	textures["disk"] = texture("textures/stone.jpg");
	textures["cylinder"] = texture("textures/snow.jpg");
	textures["sphere"] = texture("textures/heightmap.jpg");
	textures["torus"] = texture("textures/grass.jpg");
	textures["sword"] = texture("textures/sword.jpg");
	textures["crow"] = texture("textures/black.jpg");


	//Set mesh materials
	meshes["box"].get_material().set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	meshes["box"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["box"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["box"].get_material().set_shininess(25.0f);

	meshes["tetra"].get_material().set_diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	meshes["tetra"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["tetra"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["tetra"].get_material().set_shininess(25.0f);


	meshes["pyramid"].get_material().set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	meshes["pyramid"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["pyramid"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["pyramid"].get_material().set_shininess(25.0f);

	meshes["disk"].get_material().set_diffuse(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	meshes["disk"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["disk"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["disk"].get_material().set_shininess(25.0f);

	meshes["cylinder"].get_material().set_diffuse(vec4(1.0f, 0.0f, 1.0f, 1.0f));
	meshes["cylinder"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["cylinder"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["cylinder"].get_material().set_shininess(25.0f);


	meshes["sphere"].get_material().set_diffuse(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	meshes["sphere"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["sphere"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["sphere"].get_material().set_shininess(25.0f);

	meshes["torus"].get_material().set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["torus"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["torus"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["torus"].get_material().set_shininess(25.0f);

	meshes["sword"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["sword"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["sword"].get_material().set_shininess(10.0f);

	meshes["crow"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["crow"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["crow"].get_material().set_shininess(25.0f);

	// Set spot properties
	spot.set_position(vec3(30.0f, 20.0f, 0.0f));
	spot.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spot.set_direction(normalize(-spot.get_position()));
	spot.set_range(500.0f);
	spot.set_power(10.0f);

	// Load in shaders
	eff.add_shader("54_Shadowing/shader.vert", GL_VERTEX_SHADER);
	vector<string> frag_shaders{ "54_Shadowing/shader.frag", "shaders/part_spot.frag", "shaders/part_shadow.frag" };
	eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

	shadow_eff.add_shader("50_Spot_Light/spot.vert", GL_VERTEX_SHADER);
	shadow_eff.add_shader("50_Spot_Light/spot.frag", GL_FRAGMENT_SHADER);

	// Build effect
	eff.build();
	shadow_eff.build();

	// free camera properties
	cam.set_position(vec3(0.0f, 10.0f, 0.0f));
	cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

	return true;
}

bool update(float delta_time) {

	float speed = 10.0f;

	double current_x;
	double current_y;

	static const float screenHeight = static_cast<float>(renderer::get_screen_height());
	static const float screenWidth = static_cast<float>(renderer::get_screen_height());

	static const double ratio_width = quarter_pi<float>() / screenWidth;
	static const double ratio_height = (quarter_pi<float>() * (screenHeight / screenWidth)) / screenHeight;

	// Get the current cursor position
	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);

	// Calculate delta of cursor positions from last frame
	double delta_x = current_x - cursor_x;
	double delta_y = current_y - cursor_y;

	// Multiply deltas by ratios - gets actual change in orientation
	delta_x = delta_x * ratio_width;
	delta_y = delta_y * ratio_height;

	// Rotate the sphere
	meshes["sphere"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);

	// Use keyboard to rotate spotlight - QE rotate on z-axis
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_Q)) {
		spot.rotate(vec3(0.0f, 0.0f, -pi<float>() * delta_time));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_E)) {
		spot.rotate(vec3(0.0f, 0.0f, pi<float>() * delta_time));
	}

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
		spot.move(vec3(0, 0, -delta_time * speed));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
		spot.move(vec3(0, 0, delta_time * speed));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT)) {
		spot.move(vec3(-delta_time * speed, 0, 0));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT)) {
		spot.move(vec3(delta_time * speed, 0, 0));
	}
	
	//free camera 

		// Rotate cameras by delta
		// delta_y - x-axis rotation
		// delta_x - y-axis rotation
	cam.rotate(delta_x, -delta_y);
	// Use keyboard to move the camera - WSAD

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W)) {
		cam.move(vec3(0, 0, delta_time * speed));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S)) {
		cam.move(vec3(0, 0, -delta_time * speed));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A)) {
		cam.move(vec3(-delta_time * speed, 0, 0));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D)) {
		cam.move(vec3(delta_time*speed, 0, 0));
	}

	// Update the cameras
	cam.update(delta_time);

	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;


	// Update the shadow map light_position from the spot light
	shadow.light_position = spot.get_position();

	// do the same for light_dir property
	shadow.light_dir = spot.get_direction();

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

	mat4 LightProjectionMat = perspective<float>(90.f, renderer::get_screen_aspect(), 0.1f, 1000.f);

	// Bind shader
	renderer::bind(shadow_eff);
	// Render meshes
	for (auto &e : meshes) {
		auto m = e.second;
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
	}
	// *********************************
	// Set render target back to the screen
	renderer::set_render_target();

	// Set face cull mode to back
	glCullFace(GL_BACK);

	// *********************************

	// Bind shader
	renderer::bind(eff);

	// Render meshes
	for (auto &e : meshes) {
		auto m = e.second;
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = cam.get_view();
		auto P = cam.get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
			1,                                    // Number of values - 1 mat4
			GL_FALSE,                             // Transpose the matrix?
			value_ptr(MVP));                      // Pointer to matrix data
// Set M matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		// Set N matrix uniform
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE,
			value_ptr(m.get_transform().get_normal_matrix()));
		// *********************************
		// Set lightMVP uniform, using:
		 //Model matrix from m
		auto lM = m.get_transform().get_transform_matrix();
		// viewmatrix from the shadow map
		auto lV = shadow.get_view();

		// Multiply together with LightProjectionMat
		auto lMVP = LightProjectionMat * lV * lM;

		// Set uniform
		glUniformMatrix4fv(eff.get_uniform_location("lMVP"), // Location of uniform
			1,                                    // Number of values - 1 mat4
			GL_FALSE,                             // Transpose the matrix?
			value_ptr(lMVP));                      // Pointer to matrix data
		// Bind material
		renderer::bind(m.get_material(), "mat");

		// Bind spot light
		renderer::bind(spot, "spot");

		// Bind and set textures
		string mesh_name = e.first;
		texture tex = textures[mesh_name];

		// Bind texture
		renderer::bind(tex, 0);

		// Set tex uniform
		glUniform1i(eff.get_uniform_location("tex"), 0);

		// Set eye position
		vec3 eye_pos(cam.get_position());
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(eye_pos));

		// Bind shadow map texture - use texture unit 1
		renderer::bind(shadow.buffer->get_depth(), 1);

		// Set the shadow_map uniform
		glUniform1i(eff.get_uniform_location("shadow"), 1);

		// Render mesh
		renderer::render(m);

		// *********************************
	}

	return true;
}


void main() {
	// Create application
	app application("001_Coursework");
	// Set load content, update and render methods
	application.set_load_content(load_content);
	application.set_initialise(initialise);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}