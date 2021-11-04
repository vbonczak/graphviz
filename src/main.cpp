
#include "cgp/cgp.hpp" // Give access to the complete CGP library
#include <iostream> 

#define TROIS_DIMENSION
// Custom scene of this code
#ifdef TROIS_DIMENSION
#include "scene3d.hpp"
#else
#include "scene.hpp"
#endif
#include "utils.h"

// *************************** //
// Global Variables
// *************************** //


// A helper tool to factorize common default functions (Window initialization, code that starts frame, etc)
cgp::helper_common_scene helper_common;

// A storage for the current values of the inputs (mouse, keyboard, window dimension) that can be use for interaction purpose
cgp::inputs_interaction_parameters inputs;

// The custom structure of the current scene defined in "scene.hpp"
scene_structure scene;

// *************************** //
// Start of the program
// *************************** //

GLFWwindow* standard_window_initialization(int width, int height);

int main(int, char* argv[])
{
	std::cout << "Run " << argv[0] << std::endl;


	// ************************ //
	//     INITIALISATION
	// ************************ //

	// Standard Initialization with dimension in pixels
	GLFWwindow* window = standard_window_initialization(1280, 1080);


	// Custom scene initialization
	std::cout << "Initialize data of the scene ..." << std::endl;
	scene.initialize();
	std::cout << "Initialization success" << std::endl;


	// ************************ //
	//     Animation Loop
	// ************************ //
	std::cout << "Start animation loop ..." << std::endl;
	while (!glfwWindowShouldClose(window))
	{
		// Reset the screen for a new frame
		helper_common.frame_begin(scene.environment.background_color, window, inputs.window, inputs.mouse.on_gui);
		scene.environment.projection.update_aspect_ratio(inputs.window.aspect_ratio());

		// Display the ImGUI interface (button, sliders, etc)
		scene.display_gui();

		// Call the display of the scene
		scene.display();

		// End of ImGui display and handle GLFW events
		helper_common.frame_end(window);
	}

	// Cleanup
	cgp::imgui_cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}



// This function is called everytime the window is resized
void window_size_callback(GLFWwindow*, int width, int height)
{
	inputs.window = { width, height };
}


/// <summary>
/// La souris est déplacée : gestion du déplacement de l'extrémité de la queue.
/// </summary>
/// <param name="window"></param>
/// <param name="xpos"></param>
/// <param name="ypos"></param>
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	inputs.mouse_position_update({ xpos, ypos });

	const bool mouse_click_left = glfw_mouse_pressed_left(window);
	//const bool key_shift = glfw_key_shift_pressed(window);
	if (mouse_click_left && scene.cur_control != -1)
	{
		//Dans le plan de la caméra

		// Get vector orthogonal to camera orientation
		const mat4 M = scene.environment.camera.matrix_frame();
#ifndef TROIS_DIMENSION
		const vec3 n = { 0,1,0 }; 
#else
		const vec3 n = { M(0,2),M(1,2),M(2,2) };
#endif
		 
		// Compute intersection between current ray and the plane orthogonal to the view direction and passing by the selected object
		const vec2 cursor = glfw_cursor_coordinates_window(window);

		// Droite souris->tout droit
		vec3 pos, dir, intersct;
		droite_souris(scene, cursor, pos, dir);
		// Variables necessary in switch case 1:
		float r, mindist, maxdist;
		vec3 cue_dir;
		switch (scene.cur_control)
		{
		case 1:
 
			intersection_plan(intersct, pos, dir, n, scene.ctrl_pos);
 
			scene.ctrl_pos = intersct;
			r = scene.boules[0].r;
#ifndef TROIS_DIMENSION
			scene.ctrl_pos[1] = 0;
#endif
			cue_dir = scene.ctrl_pos - scene.boules[0].p;
#ifdef TROIS_DIMENSION
			scene.phi = asin(cue_dir[1] / norm(cue_dir));
			scene.theta = (cue_dir[0] >= 0 ? 1 : -1) * acos(cue_dir[2] / (cos(scene.phi) * norm(cue_dir)));
#else
			scene.theta = (cue_dir[0] >= 0 ? 1 : -1) * acos(cue_dir[2] / norm(cue_dir));
#endif
			mindist = scene.queue_length + r;
			maxdist = 2 * scene.queue_length + r;
			scene.cue_white_dist = norm(cue_dir) < mindist ? 0 : (norm(cue_dir) > maxdist ? scene.queue_length : norm(cue_dir)-scene.queue_length-r);
			scene.refresh_control_positions();
			break;
			break;
		default:
			break;
		}
	}
	else
	{
		// Default trackball mode - change this behaviour as you wish
		//camera_standard_behavior_rotation_spherical_coordinates(scene.environment.camera, inputs);
		camera_standard_behavior_rotation_trackball(scene.environment.camera, inputs);
	}
}

// This function is called everytime a mouse button is clicked/released
void mouse_click_callback(GLFWwindow* window, int button, int action, int /*mods*/)
{

	inputs.mouse.click.update_from_glfw_click(button, action);

	//Code extrait de VCL
	// 
	// Check that the mouse is clicked (drag and drop)
	const bool mouse_click_left = glfw_mouse_pressed_left(window);
	//const bool key_shift = glfw_key_shift_pressed(window);
	const vec2 cursor = glfw_cursor_coordinates_window(window);
	 
	if (mouse_click_left)
	{
		// Droite souris->sphère
		vec3 pos, dir;
		droite_souris(scene, cursor, pos, dir);

		float distance_min = 0.1f;

		vec3 intersect;
		if (intersection_droite(intersect, pos, dir, scene.ctrl_pos, scene.control_radius*3)) //controle
		{
			const float distance = norm(intersect - pos);
			if (scene.cur_control == -1 || distance < distance_min)
			{
				distance_min = distance;
				scene.cur_control = 1;
			}
		} 
		else
			scene.cur_control = -1;

	}
	else
		scene.cur_control = -1;
}

// This function is called everytime a keyboard touch is pressed/released
void keyboard_callback(GLFWwindow* /*window*/, int key, int, int action, int /*mods*/)
{
	inputs.keyboard.update_from_glfw_key(key, action);

	if (key == 32 && !scene.queue_waiting)
	{
		scene.shoot_ball();
	}
}

// Standard initialization procedure
GLFWwindow* standard_window_initialization(int width, int height)
{
	// Update storage for window size for the scene
	inputs.window = { width, height };

	// Create the window using GLFW
	GLFWwindow* window = cgp::create_window(width, height);

	// Display debug information on command line
	std::cout << cgp::opengl_info_display() << std::endl;

	// Initialize ImGUI
	cgp::imgui_init(window);

	// Set the callback functions for the inputs
	glfwSetKeyCallback(window, keyboard_callback);            // Event called when a keyboard touch is pressed/released
	glfwSetMouseButtonCallback(window, mouse_click_callback); // Event called when a button of the mouse is clicked/released
	glfwSetCursorPosCallback(window, mouse_move_callback);    // Event called when the mouse is moved
	glfwSetWindowSizeCallback(window, window_size_callback);  // Event called when the window is rescaled              

	// Load default shader and initialize default frame
	helper_common.initialize();

	return window;
}


