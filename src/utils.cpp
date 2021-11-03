#include "utils.h"

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
