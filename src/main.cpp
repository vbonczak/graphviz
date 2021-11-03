
#include "cgp/cgp.hpp" // Give access to the complete CGP library
#include <iostream> 

// Custom scene of this code
#include "scene.hpp"


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
bool intersection_plan(vec3& intersect, const vec3& droite_position, const vec3& droite_direction, const vec3& normal, const vec3& sphere_position);

vec2 glfw_cursor_coordinates_window(GLFWwindow* window)
{
	// Window size
	int w = 0, h = 0;
	glfwGetWindowSize(window, &w, &h);

	// Current cursor position
	double xpos = 0, ypos = 0;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Convert pixel coordinates to relative screen coordinates between [-1,1]
	const float x = 2 * float(xpos) / float(w) - 1;
	const float y = 1 - 2 * float(ypos) / float(h);

	return { x,y };
}
bool glfw_mouse_pressed_left(GLFWwindow* window)
{
	return (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
}
bool glfw_key_shift_pressed(GLFWwindow* window)
{
	return (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT));
}
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
void droite_souris(const vec2& cursor, vec3& pos, vec3& direction)
{
	pos = scene.environment.camera.position();

	vec4 dir_screen = { cursor.x,cursor.y,-1.0f,1.0f };
	mat4 Proj_inv = scene.environment.projection.matrix_inverse();
	mat4 View_inv = scene.environment.camera.matrix_frame();

	vec4 dir_eye = Proj_inv * dir_screen;
	vec4 dir = View_inv * vec4(dir_eye.x, dir_eye.y, -1.0f, 0.0f);

	direction = normalize(vec3(dir[0], dir[1], dir[2]));
}

// This function is called everytime the mouse is moved
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	inputs.mouse_position_update({ xpos, ypos });

	

	const bool mouse_click_left = glfw_mouse_pressed_left(window);
	const bool key_shift = glfw_key_shift_pressed(window);
	if (mouse_click_left && key_shift && scene.cur_control != -1)
	{
		//Dans le plan de la caméra

		// Get vector orthogonal to camera orientation
		const mat4 M = scene.environment.camera.matrix_frame();
		const vec3 n = { M(0,2),M(1,2),M(2,2) };

		// Compute intersection between current ray and the plane orthogonal to the view direction and passing by the selected object
		const vec2 cursor = glfw_cursor_coordinates_window(window);

		// Droite souris->sphère
		vec3 pos, dir, intersct;
		droite_souris(cursor, pos, dir);
		switch (scene.cur_control)
		{
		case 1:
			intersection_plan(intersct, pos, dir, n, scene.ctrl1);
			scene.ctrl1 = intersct;
			scene.refresh_control_positions();
			break;
		case 2:
			intersection_plan(intersct, pos, dir, n, scene.ctrl2);
			scene.ctrl2 = intersct;
			scene.refresh_control_positions();

			break;
		default:
			break;
		}
		// translate the position
		


	}
	else
	{
		// Default trackball mode - change this behavior as you wish
	//camera_standard_behavior_rotation_spherical_coordinates(scene.environment.camera, inputs);
		camera_standard_behavior_rotation_trackball(scene.environment.camera, inputs);
	}
}




bool intersection_droite(vec3& intersect, const vec3& droite_position, const vec3& droite_direction, const vec3& center, float radius)
{
	const vec3 d = droite_position - center;
	const float b = dot(droite_direction, d);
	const float c = dot(d, d) - radius * radius;

	const float delta = b * b - c;
	if (delta >= 0)
	{
		const float t0 = -b - std::sqrt(delta);
		const float t1 = -b + std::sqrt(delta);

		const float t = t0 > 0 ? t0 : t1;

		if (t > 0) {
			intersect = droite_position + t * droite_direction;
			//pick.normal = normalize(pick.intersection - center);
			return true;
		}
	}

	return false;
}

bool intersection_plan(vec3& intersect, const vec3& droite_position, const vec3& droite_direction, const vec3& normal, const vec3& sphere_position)
{
	const float t = -dot(droite_position - sphere_position, normal) / dot(droite_direction, normal);
	if (t > 0)
	{
		intersect = droite_position + t * droite_direction;
		return true;
	}

	return false;
}

// This function is called everytime a mouse button is clicked/released
void mouse_click_callback(GLFWwindow* window, int button, int action, int /*mods*/)
{

	inputs.mouse.click.update_from_glfw_click(button, action);

	//Code extrait de VCL
	// 
	// Check that the mouse is clicked (drag and drop)
	const bool mouse_click_left = glfw_mouse_pressed_left(window);
	const bool key_shift = glfw_key_shift_pressed(window);
	const vec2 cursor = glfw_cursor_coordinates_window(window);

	// Check if shift key is pressed
	if (mouse_click_left && key_shift)
	{
		// Droite souris->sphère
		vec3 pos, dir;
		droite_souris(cursor, pos, dir);

		float distance_min = 0.0f;

		vec3 intersect;
		if (intersection_droite(intersect, pos, dir, scene.ctrl1, scene.control_radius)) //control1
		{
			const float distance = norm(intersect - pos);
			if (scene.cur_control == -1 || distance < distance_min)
			{
				distance_min = distance;
				scene.cur_control = 1;
			}
		}
		else if (intersection_droite(intersect, pos, dir, scene.ctrl2, scene.control_radius)) //control2
		{
			const float distance = norm(intersect - pos);
			if (scene.cur_control == -1 || distance < distance_min)
			{
				distance_min = distance;
				scene.cur_control = 2;
			}
		}
		else
			scene.cur_control = -1;

	}
}

// This function is called everytime a keyboard touch is pressed/released
void keyboard_callback(GLFWwindow* /*window*/, int key, int, int action, int /*mods*/)
{
	inputs.keyboard.update_from_glfw_key(key, action);
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


