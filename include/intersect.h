#pragma once
#include <Eigen/Dense>
#include "material.h"
class Intersect {
public:
	bool happened;
	float dis;
	Eigen::Vector3f xyz;
	Eigen::Vector3f normal;
	Material material;

	Intersect() {
		happened = false;
		dis = 1.0e6;
	}
};