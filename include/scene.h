#pragma once
#include <Eigen/Dense>
#include <vector>
#include "object.h"
class Scene {
public:
	Eigen::Vector3f backgroundcolor={ 0.235294f, 0.67451f, 0.843137f };
	std::vector<Object*> object_list;
	float RussianRoulette = 0.8f;

	void add(Object* object) { object_list.push_back(object); }
	float sample_light(Intersect& inter_light);
};