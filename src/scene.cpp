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

	init_objects();

	timer.event_period = 0.1f;
	timer.scale = 0.05f;


	alpha = 0.98f;
	beta = 0.05;
	mu = 0.92f;
	vepsilon = 0.30f;

}

void scene_structure::init_objects()
{
	
	mesh plane_mesh = mesh_primitive_quadrangle({ 0, 0, 0 },
		{ H, 0, 0 },
		{ H, 0, L },
		{ 0, 0, L });

	plane.initialize(plane_mesh, "plan");

	//Les bords
	mesh bord_mesh = mesh_primitive_quadrangle({ -H / 2, 0, -L / 2 }, { -H / 2, h , -L / 2 }, { H / 2, h, -L / 2 }, { H / 2, 0, -L / 2 });
	bord1.initialize(bord_mesh, "bord1");
	bord2.initialize(bord_mesh, "bord2");
	bord2.transform.translation = { 0, 0, L };
	bord_mesh = mesh_primitive_quadrangle({ -H / 2, 0, -L / 2 }, { -H / 2, h , -L / 2 }, { -H / 2, h, L / 2 }, { -H / 2, 0, L / 2 });
	bord3.initialize(bord_mesh, "bord3");
	bord4.initialize(bord_mesh, "bord4");
	bord4.transform.translation = { H, 0, 0 };
	 
	texture_bois = opengl_load_texture_image("assets/bois.jpg");
	bord4.texture = bord3.texture = bord2.texture = bord1.texture = texture_bois;

	sphere.shading.color = { 0.8f, 0.5f, 0.7f };
	sphere.shading.phong.specular = 0.85f;
	plane.shading.color = { 0.0f, 0x6d / 255.f, 0.0f };
	plane.shading.phong.specular = .0f;
	plane.transform.translation = { -H / 2,0,-L / 2 };
	control_radius = 0.06f;



	//Contr�les
	control_sphere.initialize(mesh_primitive_sphere(control_radius), "ctrl");
	control_sphere.shading.color = { 1,0,0 };

	sphere.initialize(mesh_primitive_sphere(), "Sphere");

	add_balls();

	ctrl_pos = { 3,0,3 };
	queue_radius = 0.02f;
	queue_init();
	queue_waiting = true;

	control_sphere.transform.translation = ctrl_pos;

	cur_control = -1;
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

	draw(bord1, environment);
	draw(bord2, environment);
	draw(bord3, environment);
	draw(bord4, environment);

	// Call the simulation of the particle system
	float const dt = 0.01f * timer.scale;
	simulate(boules, dt);

	// Display the result
	sphere_display();

	draw(queue, environment);
	if (cur_control == 1)
	{
		control_sphere.shading.color = { 0,0,1 };
	}
	else
	{
		control_sphere.shading.color = { 1,0,0 };
	}

	draw(control_sphere, environment);

	if (gui.display_frame)
		draw(global_frame, environment);

	if (queue_waiting && norm(boules[0].v) <= 0.1f)
	{
		queue_reinit();
		queue_waiting = false;
	}
}

void scene_structure::sphere_display()
{
	// Display the particles as spheres
	size_t const N = boules.size();
	for (size_t k = 0; k < N; ++k)
	{
		particle_structure const& particle = boules[k];
		//sphere.texture = particle.text;
		sphere.transform.translation = particle.p;
		sphere.transform.scaling = particle.r;
		sphere.shading.color = particle.c;
		draw(sphere, environment);
	}
}



void scene_structure::refresh_control_positions()
{
	control_sphere.transform.translation = ctrl_pos;
	queue_reinit();
}

void scene_structure::queue_init()
{
	queue.initialize(mesh_primitive_cylinder(queue_radius, ctrl_pos, boules[0].p), "queue");
	queue.shading.color = { 0x63 / 255.f,0,0 };
}

void scene_structure::queue_reinit()
{
	queue.clear();
	queue_init();
}

void scene_structure::launch_ball()
{
	//Lancement de la boule, sa vitesse devient >0

	//ensuite on place ce param�tre pour replacer correctement la queue quand la boule s'immobilisera
	queue_waiting = true;
}

buffer<vec3> const colors = { {1,1,1},//blanche
	{0xFF / 255.,0xBF / 255.,0},
{0x0C / 255.,0x35 / 255.,0xff / 255.},
	{0xff / 255,0 ,0x0c / 255.},
	{0x0C / 255.,0x0E / 255.,0xAD / 255.},

	{0xFF / 255.,0x65 / 255.,0x0C / 255.},
	{0x00 / 255.,0xCE / 255.,0x78 / 255.},
	{0xB2 / 255.,0x08 / 255.,0x00 / 255.},
	{0x00 / 255.,0x00 / 255.,0x00 / 255.},//8 NOIRE

	 {0xFF / 255.,0xBF / 255.,0},
	{0x0C / 255.,0x35 / 255.,0xff / 255.},
	{0xff / 255,0 ,0x0c / 255},
	{0x0C / 255.,0x0E / 255.,0xAD / 255.},

	{0xFF / 255.,0x65 / 255.,0x0C / 255.},
	{0x00 / 255.,0xCE / 255.,0x78 / 255.},
	{0xB2 / 255.,0x08 / 255.,0x00 / 255.},
};

void scene_structure::add_balls()
{
	// Emit particle with random velocity
	//  Assume first that all particles have the same radius and mass
	for (size_t i = 0; i < colors.size(); i++)
	{
		//balls_textures.push_back(opengl_load_texture_image("assets/" + to_string(i % 7 + 1) + ".jpg"));
		
		float const theta = rand_interval(0, 2 * pi);
		vec3 const v = vec3(5.0f * std::cos(theta), 50.0f, 5.0f * std::sin(theta));

		particle_structure particle;
		particle.p = { 0,1,0 };
		particle.r = 0.03f;//6cm de diam�tre
		particle.c = colors[i];
		//particle.text = balls_textures[i];
		particle.v = v;
		particle.m = 200.0f; //200g

		boules.push_back(particle);
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