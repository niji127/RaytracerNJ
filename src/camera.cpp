#include "camera.h"
#include "global.h"

Ray Camera::cast_ray(const Eigen::Vector2f& p) {
	float width_length = 2 * p.x() / (float)width - 1.0f;
	float height_length = 2 * p.y() / (float)height - 1.0f;
	Eigen::Vector3f ray_direction = { -tanf(0.5f * fov * PI / 180.0f) * width_length,
	tanf(0.5f * fov * PI / 180.0f) * height_length,
	1.0f };
	return Ray(view_point,ray_direction);
}