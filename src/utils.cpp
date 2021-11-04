#include "utils.h"

//Fonctions issues de la bibliothèque VCL de Damien Rohmer https://github.com/drohmer/inf443-vcl
//Adaptées pour les besoins du programme.

/// <summary>
/// Retourne la droite curseur - fond de la scène de la caméra actuelle
/// via les paramètres par référence pos et direction
/// </summary>
/// <param name="scene"></param>
/// <param name="cursor"></param>
/// <param name="pos"></param>
/// <param name="direction"></param>
void droite_souris(const scene_structure& scene, const vec2& cursor, vec3& pos, vec3& direction)
{
	pos = scene.environment.camera.position();

	vec4 dir_screen = { cursor.x,cursor.y,-1.0f,1.0f };
	mat4 Proj_inv = scene.environment.projection.matrix_inverse();
	mat4 View_inv = scene.environment.camera.matrix_frame();

	vec4 dir_eye = Proj_inv * dir_screen;
	vec4 dir = View_inv * vec4(dir_eye.x, dir_eye.y, -1.0f, 0.0f);

	direction = normalize(vec3(dir[0], dir[1], dir[2]));
}

/// <summary>
/// Retourne true si le droite coupe la sphère.
/// intersect est le point d'intersection
/// </summary>
/// <param name="intersect"></param>
/// <param name="droite_position"></param>
/// <param name="droite_direction"></param>
/// <param name="center"></param>
/// <param name="radius"></param>
/// <returns></returns>
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
			return true;
		}
	}

	return false;
}

/// <summary>
/// Intersection d'une droite et d'un plan auquel appartient sphere_position.
/// </summary>
/// <param name="intersect"></param>
/// <param name="droite_position"></param>
/// <param name="droite_direction"></param>
/// <param name="normal"></param>
/// <param name="sphere_position"></param>
/// <returns>True si une intersection existe.</returns>
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



vec2 glfw_cursor_coordinates_window(GLFWwindow* window)
{
	// Window size
	int w = 0, _h = 0;
	glfwGetWindowSize(window, &w, &_h);

	// Current cursor position
	double xpos = 0, ypos = 0;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Convert pixel coordinates to relative screen coordinates between [-1,1]
	const float x = 2 * float(xpos) / float(w) - 1;
	const float y = 1 - 2 * float(ypos) / float(_h);

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