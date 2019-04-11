#include <glm/glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh sphere;
mesh skybox;
effect eff;
effect sky_eff;
cubemap cube_map;
texture tarnish;
target_camera cam;

bool load_content() {
  // *********************************
  // Create a sphere
	sphere = mesh(geometry_builder::create_sphere(25, 25));
	// Create box geometry for skybox
	skybox = mesh(geometry_builder::create_box());
	// Scale box by 100 - allows a distance
	skybox.get_transform().scale = vec3(100.0f);
	// Load the cubemap  - create array of six filenames +x, -x, +y, -y, +z, -z
	array<string, 6> filenames = { "textures/sahara_ft.jpg", "textures/sahara_bk.jpg", "textures/sahara_up.jpg",
								  "textures/sahara_dn.jpg", "textures/sahara_rt.jpg", "textures/sahara_lf.jpg" };
	// Create cube_map
	cube_map = cubemap(filenames);

  // Load in tarnish texture
	tarnish = texture("textures/tarnish.jpg");
  
	// Load in environment map shader
	eff.add_shader("58_Environment_Maps/env_map.vert", GL_VERTEX_SHADER);
	eff.add_shader("58_Environment_Maps/env_map.frag", GL_FRAGMENT_SHADER);

  // Build effect
	eff.build();

  // Load in skybox effect
		sky_eff.add_shader("shaders/skybox.vert", GL_VERTEX_SHADER);
	sky_eff.add_shader("shaders/skybox.frag", GL_FRAGMENT_SHADER);

	// Build effect
	sky_eff.build();
  // *********************************
  // Set camera properties
  cam.set_position(vec3(0.0f, 0.0f, 10.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  cam.update(delta_time);
  // *********************************
  // Set skybox position to camera position (camera in centre of skybox)
  skybox.get_transform().position = cam.get_position();

  // rotate the sphere
  sphere.get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);

  // *********************************
  return true;
}

bool render() {
  // *********************************
	// Disable depth test,depth mask,face culling
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

  // Bind skybox effect
	renderer::bind(sky_eff);

  // Calculate MVP for the skybox
	auto M = skybox.get_transform().get_transform_matrix();
	auto V = cam.get_view();
	auto P = cam.get_projection();
	auto MVP = P * V * M;

	// Set MVP matrix uniform
	glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

	// Bind cubemap to TU 0
	renderer::bind(cube_map, 0);

	// Set cubemap uniform
	glUniform1i(eff.get_uniform_location("cubemap"), 0);
	//TODO this doesnt work why 

	// Render skybox
	renderer::render(skybox);

	// Enable depth test,depth mask,face culling
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);


	// Bind effect
	renderer::bind(eff);

// Create MVP matrix
	M = sphere.get_transform().get_transform_matrix();
	V = cam.get_view();
	P = cam.get_projection();
	MVP = P * V * M;



	// Set MVP matrix uniform
	glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

	// Set M matrix uniform
	glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));

	// Set N matrix uniform
	mat3 N = sphere.get_transform().get_normal_matrix();
	glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));

	// Set eye_pos value
	vec3 eye_pos(cam.get_position());
	glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(eye_pos));

	// Bind cubemap to TU 0
	renderer::bind(cube_map, 0);

	// Set cubemap uniform
	glUniform1i(eff.get_uniform_location("cube_map"), 0);

  // Bind tarnish to TU 1
	renderer::bind(tarnish, 1);

  // Set tarnish uniform to 1
	glUniform1i(eff.get_uniform_location("tarnish"), 1);

  // *********************************
  // Render mesh
  renderer::render(sphere);

  return true;
}

void main() {
  // Create application
  app application("59_Tarnished_Object");
  // Set methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}