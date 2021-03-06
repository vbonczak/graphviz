#ifdef TROIS_DIMENSION
#include "scene3d.hpp"


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

	theta = -pi;

	phi = 0;

	cue_white_dist = 0;

	alpha = 0.98f;
	beta = 0.95f;
	beta_feutre = 0.1f;
	mu = 0.99f;
	vepsilon = 0.30f;

}

void scene_structure::init_objects()
{
	// Edges of the containing cube
	//  Note: this data structure is set for display purpose - don't use it to compute some information on the cube - it would be un-necessarily complex
	buffer<vec3> cube_wireframe_data;
	//buffer_stack<float, 6> bounds = { lower_bounds[0],lower_bounds[1],lower_bounds[2], upper_bounds[0],upper_bounds[1],upper_bounds[2] };

	lower_bounds = { -H / 2, -H / 2, -L / 2 };
	upper_bounds = { H / 2, H / 2, L / 2 };

	cube_wireframe_data = { {lower_bounds[0],lower_bounds[1],lower_bounds[2]},{upper_bounds[0],lower_bounds[1],lower_bounds[2]}, {upper_bounds[0],lower_bounds[1],lower_bounds[2]},{upper_bounds[0],upper_bounds[1],lower_bounds[2]}, {upper_bounds[0],upper_bounds[1],lower_bounds[2]},{lower_bounds[0],upper_bounds[1],lower_bounds[2]}, {lower_bounds[0],upper_bounds[1],lower_bounds[2]},{lower_bounds[0],lower_bounds[1],lower_bounds[2]},
	   {lower_bounds[0],lower_bounds[1],upper_bounds[2]} ,{upper_bounds[0],lower_bounds[1],upper_bounds[2]},  {upper_bounds[0],lower_bounds[1],upper_bounds[2]}, {upper_bounds[0],upper_bounds[1],upper_bounds[2]},  {upper_bounds[0],upper_bounds[1],upper_bounds[2]}, {lower_bounds[0],upper_bounds[1],upper_bounds[2]},  {lower_bounds[0],upper_bounds[1],upper_bounds[2]}, {lower_bounds[0],lower_bounds[1],upper_bounds[2]},
	   {lower_bounds[0],lower_bounds[1],lower_bounds[2]},{lower_bounds[0],lower_bounds[1],upper_bounds[2]}, {upper_bounds[0],lower_bounds[1],lower_bounds[2]},{upper_bounds[0],lower_bounds[1],upper_bounds[2]}, {upper_bounds[0],upper_bounds[1],lower_bounds[2]},{upper_bounds[0],upper_bounds[1],upper_bounds[2]},   {lower_bounds[0],upper_bounds[1],lower_bounds[2]},{lower_bounds[0],upper_bounds[1],upper_bounds[2]} };

	cube_wireframe.initialize(cube_wireframe_data, "cube wireframe");

	mesh hole = mesh_primitive_sphere(0.06f, { -H / 2,-H/2,-L / 2 });
	hole1.initialize(hole, "hole1");
	hole2.initialize(hole, "hole2");
	hole3.initialize(hole, "hole3");
	hole4.initialize(hole, "hole4");
	hole5.initialize(hole, "hole5");
	hole6.initialize(hole, "hole6");
	hole7.initialize(hole, "hole7");
	hole8.initialize(hole, "hole8");
	hole9.initialize(hole, "hole9");
	hole10.initialize(hole, "hole10");
	hole11.initialize(hole, "hole11");
	hole12.initialize(hole, "hole12");
	hole2.transform.translation = { 0,0,L / 2 };
	hole3.transform.translation = { 0,0,L };
	hole4.transform.translation = { H,0,0 };
	hole5.transform.translation = { H,0,L / 2 };
	hole6.transform.translation = { H,0,L };
	hole7.transform.translation =  { 0, H,0 };
	hole8.transform.translation = { 0, H, L / 2 };
	hole9.transform.translation = { 0, H, L };
	hole10.transform.translation = { H, H,0 };
	hole11.transform.translation = { H, H,L / 2 };
	hole12.transform.translation = { H, H,L };
		
	hole1.shading.color = { 0,0,0 };
	hole2.shading.color = { 0,0,0 };
	hole3.shading.color = { 0,0,0 };
	hole4.shading.color = { 0,0,0 };
	hole5.shading.color = { 0,0,0 };
	hole6.shading.color = { 0,0,0 };
	hole7.shading.color = { 0,0,0 };
	hole8.shading.color = { 0,0,0 };
	hole9.shading.color = { 0,0,0 };
	hole10.shading.color = { 0,0,0 };
	hole11.shading.color = { 0,0,0 };
	hole12.shading.color = { 0,0,0 };


	 
	sphere.shading.color = { 0.8f, 0.5f, 0.7f };
	sphere.shading.phong.specular = 0.85f;
	 control_radius = 0.06f;

	//Contr?les
	control_sphere.initialize(mesh_primitive_sphere(control_radius), "ctrl");
	control_sphere.shading.color = { 1,0,0 };

	sphere.initialize(mesh_primitive_sphere(), "Sphere");

	add_balls();


	queue_radius = 0.02f;
	queue_length = 1.5f;
	queue_init();
	queue_waiting = true;

	ctrl_pos = { 0,0,-L / 4 - queue_length };
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

	draw(hole1, environment);
	draw(hole2, environment);
	draw(hole3, environment);
	draw(hole4, environment);
	draw(hole5, environment);
	draw(hole6, environment);
	draw(hole7, environment);
	draw(hole8, environment);
	draw(hole9, environment);
	draw(hole10, environment);
	draw(hole11, environment);
	draw(hole12, environment);

	// Call the simulation of the particle system
	float const dt = 0.1f * timer.scale;
	if (ImGui::Button("Shoot!") && !queue_waiting)
	{
		shoot_ball();
	}
	simulate3d(boules, dt);

	// Display the result
	sphere_display();
	if (!queue_waiting)
	{
		refresh_control_positions();
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
	}
	if (gui.display_frame)
		draw(global_frame, environment);

	if (queue_waiting && norm(boules[0].v) <= 0.1f)
	{
		queue_reinit();
		queue_waiting = false;
	}

	draw(cube_wireframe, environment);
}

void scene_structure::sphere_display()
{
	// Display the particles as spheres
	size_t const N = boules.size();
	for (size_t k = 0; k < N; ++k)
	{
		boule_structure const& boule = boules[k];
		//sphere.texture = particle.text;
		sphere.transform.translation = boule.p;
		sphere.transform.scaling = boule.r;
		sphere.shading.color = boule.c;
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
	// We want ctrl_pos to be between queue_length and 2*queue_length away from the white ball, and the cue length to be constant
	vec3 white_pos = boules[0].p;
	float r = boules[0].r;
	ctrl_pos = { white_pos[0] + cos(phi)*sin(theta) * (cue_white_dist + r + queue_length),white_pos[1]+sin(phi)* (cue_white_dist + r + queue_length),white_pos[2] + cos(phi)*cos(theta) * (cue_white_dist + r + queue_length) };
	vec3 end_pos = { white_pos[0] + cos(phi)*sin(theta) * (cue_white_dist + r),white_pos[1] + sin(phi) * (cue_white_dist + r),white_pos[2] + cos(phi)*cos(theta) * (cue_white_dist + r) };
	queue.initialize(mesh_primitive_cylinder(queue_radius, ctrl_pos, end_pos), "queue");
	queue.shading.color = { 0x63 / 255.f,0,0 };
}

void scene_structure::queue_reinit()
{
	queue.clear();
	queue_init();
}

void scene_structure::shoot_ball()
{
	boules[0].v = { -cos(phi)*sin(theta) * cue_white_dist * 20,-sin(phi) * cue_white_dist * 20,-cos(phi)*cos(theta) * cue_white_dist * 20 };
	//ensuite on place ce param?tre pour replacer correctement la queue quand la boule s'immobilisera
	queue_waiting = true;
}

buffer<vec3> const colors = { {1,1,1},//blanche
	/*{0xFF / 255.,0xBF / 255.,0},
	{0x0C / 255.,0x35 / 255.,0xff / 255.},
	{0xff / 255,0 ,0x0c / 255.},
	{0x0C / 255.,0x0E / 255.,0xAD / 255.},

	{0xFF / 255.,0x65 / 255.,0x0C / 255.},
	{0x00 / 255.,0xCE / 255.,0x78 / 255.},
	{0xB2 / 255.,0x08 / 255.,0x00 / 255.},*/
	{1,0,0},
	{1,0,0},
	{1,0,0},
	{1,0,0},
	{1,0,0},
	{1,0,0},
	{1,0,0},
	//{0x00 / 255.,0x00 / 255.,0x00 / 255.},//8 NOIRE
	{0,0,0}, //8 ball
	{1,1,0},
	{1,1,0},
	{1,1,0},
	{1,1,0},
	{1,1,0},
	{1,1,0},
	{1,1,0}
	/*{0xFF / 255.,0xBF / 255.,0},
	{0x0C / 255.,0x35 / 255.,0xff / 255.},
	{0xff / 255,0 ,0x0c / 255},
	{0x0C / 255.,0x0E / 255.,0xAD / 255.},

	{0xFF / 255.,0x65 / 255.,0x0C / 255.},
	{0x00 / 255.,0xCE / 255.,0x78 / 255.},
	{0xB2 / 255.,0x08 / 255.,0x00 / 255.},*/
};

void scene_structure::add_balls()
{
	// Emit particle with random velocity
	// Assume first that all particles have the same radius and mass
	/*for (size_t i = 0; i < colors.size(); i++)
	{
		//balls_textures.push_back(opengl_load_texture_image("assets/" + to_string(i % 7 + 1) + ".jpg"));

		//float const theta = rand_interval(0, 2 * pi);
		//vec3 const v = vec3(5.0f * std::cos(theta), 50.0f, 5.0f * std::sin(theta));

		particle_structure particle;
		particle.p = { 0,1,0 };
		particle.r = 0.03f;//6cm de diam?tre
		particle.c = colors[i];
		//particle.text = balls_textures[i];
		particle.v = { 0,0,0 };
		particle.m = 200.0f; //200g

		boules.push_back(particle);
	}*/

	boule_structure boule;
	boule.r = 0.03f;//6cm de diam?tre
	boule.p = { 0, 0, -L / 4 };
	boule.c = { 1,1,1 };
	boule.v = { 0,0,0 };
	boule.m = 0.2f; //200g
	boule.in_play = true;

	boules.push_back(boule);

	int yellows = 27, reds = 27;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < i + 1; j++)
		{
			for (int k = 0; k < i + 1; k++)
			{
				boule.p = { (2 * j - i) * boule.r, (2 * k - i) * boule.r, L / 4 + sqrt(3) * boule.r * i };
				if (i == 2 && j == 1 && k==1)
				{
					boule.c = { 0,0,0 };
					boules.push_back(boule);
					continue;
				}
				if (yellows == 0 || (reds > 0 && std::rand() % 2 == 0))
				{
					boule.c = { 1,0,0 };
					boules.push_back(boule);
					reds--;
					continue;
				}
				boule.c = { 1,1,0 };
				boules.push_back(boule);
				yellows--;
			}
		}
	}
}

void scene_structure::display_gui()
{
	//ImGui::Checkbox("Frame", &gui.display_frame);
	//ImGui::SliderFloat("Time scale", &timer.scale, 0.05f, 2.0f, "%.2f s");
	//ImGui::SliderFloat("Time to add new sphere", &timer.event_period, 0.05f, 2.0f, "%.2f s");
	//ImGui::SliderFloat("Friction", &alpha, 0.05f, 1.0f, "%.2f");
	//ImGui::SliderFloat("Impact", &beta, 0.05f, 1.0f, "%.2f");
	ImGui::SliderFloat("Cue x/y angle", &theta, -pi, pi, "%.2f");
	ImGui::SliderFloat("Cue z angle", &phi, -pi / 2, pi / 2, "%.2f");
	ImGui::SliderFloat("Cue force", &cue_white_dist, 0, queue_length, "%.2f");
	//ImGui::SliderFloat("Static friction", &mu, 0.05f, 1.0f, "%.2f");
	//ImGui::SliderFloat("Vepsilon", &vepsilon, 0.01f, 10.0f, "%.2f");  
}

#endif