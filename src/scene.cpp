#include "scene.hpp"


using namespace cgp;

void scene_structure::initialize()
{
	// Initialize the skybox (*)
	// ***************************************** //
	skybox.initialize("assets/skybox/");         // indicate a path where to load the 6 texture images

	// Basic set-up
	// ***************************************** //
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.look_at({ 3.0f,2.0f,2.0f }, { 0,0,0 }, { 0,1,0 });


	mesh plane_mesh = mesh_primitive_quadrangle({ -1.5,0,1.15 },
		{ 1.15,0,1.5 },
		{ 1.5,0,-1.15 },
		{ -1.5,0, -1.15 });

	plane.initialize(plane_mesh, "plan");

	sphere.shading.color = { 0.8f, 0.5f, 0.7f };
	sphere.shading.phong.specular = 0.85f;
	plane.shading.color = { 0.3f, 0.8f, 0.4f };
	plane.shading.phong.specular = .0f;

	timer.event_period = 0.1f;
	timer.scale = 0.1f;
	// Edges  
	sphere.initialize(mesh_primitive_sphere(), "Sphere");

	add_balls();
	
	alpha = 0.98f;
	beta = 0.05;
	mu = 0.92f;
	vepsilon = 0.30f;
}



void scene_structure::display()
{
	// Display of the skybox (*)
	// ***************************************** //
	draw(skybox, environment);
	// Note: The skybox must be drawn before the other shapes 
	//   Skybox is displayed without writing in the z-buffer.
	//   In displaying it first, the cube appears beyond any other shape.

	timer.update();
	environment.light = environment.camera.position();

	// Other elements of the scene
	// ***************************************** //

	// Basics common elements
	// ***************************************** //

	if (gui.display_wireframe)
	{
		draw_wireframe(plane, environment);
	}
	else
	{
		draw(plane, environment);
	}

	// Call the simulation of the particle system
	float const dt = 0.01f * timer.scale;
	simulate(particles, dt);

	// Display the result
	sphere_display();


	if (gui.display_frame)
		draw(global_frame, environment);

}

void scene_structure::sphere_display()
{
	// Display the particles as spheres
	size_t const N = particles.size();
	for (size_t k = 0; k < N; ++k)
	{
		particle_structure const& particle = particles[k];
		sphere.shading.color = particle.c;
		sphere.transform.translation = particle.p;
		sphere.transform.scaling = particle.r;

		draw(sphere, environment);
	}
}




buffer<vec3> const colors = { {0xFF   / 255.,0xBF   / 255.,0},
{0x0C   / 255.,0x35   / 255.,0xff   / 255.},
	{0xff   / 255,0 ,0x0c   / 255.},
	{0x0C   / 255.,0x0E   / 255.,0xAD   / 255.},

	{0xFF   / 255.,0x65   / 255.,0x0C   / 255.},
	{0x00   / 255.,0xCE   / 255.,0x78   / 255.},
	{0xB2   / 255.,0x08   / 255.,0x00   / 255.},
	{0x00   / 255.,0x00   / 255.,0x00   / 255.},//8 NOIRE

	 {0xFF   / 255.,0xBF   / 255.,0},
	{0x0C   / 255.,0x35   / 255.,0xff   / 255.},
	{0xff   / 255,0 ,0x0c   / 255},
	{0x0C   / 255.,0x0E   / 255.,0xAD   / 255.},

	{0xFF   / 255.,0x65   / 255.,0x0C   / 255.},
	{0x00   / 255.,0xCE   / 255.,0x78   / 255.},
	{0xB2   / 255.,0x08   / 255.,0x00   / 255.},
	{1,1,1},//blanche
};

void scene_structure::add_balls()
{
	// Emit particle with random velocity
	//  Assume first that all particles have the same radius and mass
	for (size_t i = 0; i < colors.size(); i++)
	{
		float const theta = rand_interval(0, 2 * pi);
		vec3 const v = vec3(5.0f * std::cos(theta), 50.0f, 5.0f * std::sin(theta));

		particle_structure particle;
		particle.p = { 0,1,0 };
		particle.r = 0.06f;//6cm
		particle.c =colors[i];
		particle.v = v;
		particle.m = 200.0f; //200g

		particles.push_back(particle);

	}

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::SliderFloat("Time scale", &timer.scale, 0.05f, 2.0f, "%.2f s");
	ImGui::SliderFloat("Time to add new sphere", &timer.event_period, 0.05f, 2.0f, "%.2f s");
	//ImGui::SliderFloat("Friction", &alpha, 0.05f, 1.0f, "%.2f");
	ImGui::SliderFloat("Impact", &beta, 0.05f, 1.0f, "%.2f");
	//ImGui::SliderFloat("Static friction", &mu, 0.05f, 1.0f, "%.2f");
	//ImGui::SliderFloat("Vepsilon", &vepsilon, 0.01f, 10.0f, "%.2f");  
}