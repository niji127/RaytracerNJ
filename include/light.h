#pragma once
#include <Eigen/Dense>
class Ray {
public:
	Ray(const Eigen::Vector3f& ori, const Eigen::Vector3f& dir)
		: origin(ori), direction(dir) {
		this->direction.normalize();
		this->direction_inv
			= { 1.0f / this->direction.x(),
			1.0f / this->direction.y(),
			1.0f / this->direction.z() };
	}

	Eigen::Vector3f origin;
	Eigen::Vector3f direction;
	Eigen::Vector3f direction_inv;
};