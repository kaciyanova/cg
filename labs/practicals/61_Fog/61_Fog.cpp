#include <glm\glm.hpp>
#include <graphics_framework.h>

// Types of fog
#define FOG_LINEAR 0
#define FOG_EXP 1
#define FOG_EXP2 2

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
effect eff;
texture tex;
free_camera cam;
double cursor_x = 0.0;
double cursor_y = 0.0;
// target_camera cam;
directional_light light;

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

  // Set materials
  // Red box
  meshes["box"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  meshes["box"].get_material().set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
  meshes["box"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  meshes["box"].get_material().set_shininess(25.0f);
  // Green tetra
  meshes["tetra"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  meshes["tetra"].get_material().set_diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f));
  meshes["tetra"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  meshes["tetra"].get_material().set_shininess(25.0f);
  // Blue pyramid
  meshes["pyramid"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  meshes["pyramid"].get_material().set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));
  meshes["pyramid"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  meshes["pyramid"].get_material().set_shininess(25.0f);
  // Yellow disk
  meshes["disk"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  meshes["disk"].get_material().set_diffuse(vec4(1.0f, 1.0f, 0.0f, 1.0f));
  meshes["disk"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  meshes["disk"].get_material().set_shininess(25.0f);
  // Magenta cylinder
  meshes["cylinder"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  meshes["cylinder"].get_material().set_diffuse(vec4(1.0f, 0.0f, 1.0f, 1.0f));
  meshes["cylinder"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  meshes["cylinder"].get_material().set_shininess(25.0f);
  // Cyan sphere
  meshes["sphere"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  meshes["sphere"].get_material().set_diffuse(vec4(0.0f, 1.0f, 1.0f, 1.0f));
  meshes["sphere"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  meshes["sphere"].get_material().set_shininess(25.0f);
  // White torus
  meshes["torus"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  meshes["torus"].get_material().set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  meshes["torus"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  meshes["torus"].get_material().set_shininess(25.0f);

  // Load texture
  tex = texture("textures/checker.png");

  // Set lighting values
  light.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
  light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  light.set_direction(vec3(1.0f, 1.0f, -1.0f));

  // Set the clear colour to be a light grey, the same as our fog.
  renderer::setClearColour(0.5f, 0.5f, 0.5f);

  // Load in shaders
  eff.add_shader("61_Fog/shader.vert", GL_VERTEX_SHADER);
  eff.add_shader("61_Fog/shader.frag", GL_FRAGMENT_SHADER);
  eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
  eff.add_shader("61_Fog/part_fog.frag", GL_FRAGMENT_SHADER);
  // Build effect
  eff.build();

  // Set camera properties
  cam.set_position(vec3(50.0f, 10.0f, 50.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}

bool update(float delta_time) {

	// The ratio of pixels to rotation - remember the fov
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

	//glfwGetCursorPos(window, x,y);
	// Calculate delta of cursor positions from last frame
	double delta_x = current_x - cursor_x;
	double delta_y = current_y - cursor_y;

	// Multiply deltas by ratios - gets actual change in orientation
	delta_x = delta_x * ratio_width;
	delta_y = delta_y * ratio_height;
	float speed = 10.0f;

	// Rotate cameras by delta
	// delta_y - x-axis rotation
	// delta_x - y-axis rotation
	cam.rotate(delta_x*speed, -delta_y * speed);
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


	// Move camera

	// Update the camera
	cam.update(delta_time);

	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;

  if (glfwGetKey(renderer::get_window(), '1')) {
    cam.set_position(vec3(50, 10, 50));
  }
  if (glfwGetKey(renderer::get_window(), '2')) {
    cam.set_position(vec3(-50, 10, 50));
  }
  if (glfwGetKey(renderer::get_window(), '3')) {
    cam.set_position(vec3(-50, 10, -50));
  }
  if (glfwGetKey(renderer::get_window(), '4')) {
    cam.set_position(vec3(50, 10, -50));
  }

  // Rotate the sphere
  meshes["sphere"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);

  cam.update(delta_time);

  return true;
}

bool render() {
  // Render meshes
  for (auto &e : meshes) {
    auto m = e.second;
    // Bind effect
    renderer::bind(eff);
    // Create MVP matrix
    auto M = m.get_transform().get_transform_matrix();
    auto V = cam.get_view();
    auto P = cam.get_projection();
    auto MVP = P * V * M;
    // Set MVP matrix uniform
    glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
    // Create MV matrix
    auto MV = V * M;
    // Set MV matrix uniform
    glUniformMatrix4fv(eff.get_uniform_location("MV"), 1, GL_FALSE, value_ptr(MV));
    // Set M matrix uniform
    glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
    // Set N matrix uniform
    glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
    // Bind material
    renderer::bind(m.get_material(), "mat");
    // Bind light
    renderer::bind(light, "light");
    // Bind texture
    renderer::bind(tex, 0);
    // Set tex uniform
    glUniform1i(eff.get_uniform_location("tex"), 0);
    // Set eye position
    glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));

    // *********************************
    // Set fog colour to the same as the clear colour
	vec4 fog_colour= vec4(0.5f, 0.5f, 0.5f,1.0f);
	glUniform4fv(eff.get_uniform_location("fog_colour"), 1, value_ptr(fog_colour));

    // Set fog start:  5.0f
	float fog_start = 5.0f;
	glUniform1f(eff.get_uniform_location("fog_start"), 1);
	// Set fog end:  100.0f
	float fog_end = 100.0f;
	glUniform1f(eff.get_uniform_location("fog_end"), 1);
	// Set fog density: 0.04f
	float fog_density = 0.04f;
	glUniform1f(eff.get_uniform_location("fog_density"), 1);
	// Set fog type: FOG_EXP2
	int fog_type = 0;
	glUniform1i(eff.get_uniform_location("fog_type"), 2);
	// *********************************
	//TODO it doesn't look riGHT
    // Render mesh
    renderer::render(m);
  }
  return true;
}

void main() {
  // Create application
  app application("61_Fog");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}