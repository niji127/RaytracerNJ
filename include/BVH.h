#pragma once
#include <vector>
#include "triangle.h"
#include "scene.h"
class BVH {
public:
	BVH* left_node;
	BVH* right_node;
	BVH* parent_node;

	Eigen::Vector3f min_pos;
	Eigen::Vector3f max_pos;
	std::vector<Triangle*> triangle_list;
	BVH(const Scene& scene) {
		Eigen::Vector3f min_xyz = { 1e6,1e6,1e6 };
		Eigen::Vector3f max_xyz = { -1e6,-1e6, -1e6 };
		for (auto iter : scene.object_list) {
			for (int i = 0; i < iter->triangle_num; i++) {
				auto t = iter->triangle_list[i];
				this->triangle_list.push_back(t);
				for (int j = 0; j < 3; j++) {
					min_xyz.x() = std::min(min_xyz.x(), t->point[j].x());
					min_xyz.y() = std::min(min_xyz.y(), t->point[j].y());
					min_xyz.z() = std::min(min_xyz.z(), t->point[j].z());

					max_xyz.x() = std::max(max_xyz.x(), t->point[j].x());
					max_xyz.y() = std::max(max_xyz.y(), t->point[j].y());
					max_xyz.z() = std::max(max_xyz.z(), t->point[j].z());
				}
			}
		}
		min_pos = min_xyz;
		max_pos = max_xyz;
		left_node = nullptr;
		right_node = nullptr;
		parent_node = nullptr;
	}

	BVH(const Eigen::Vector3f& min, const Eigen::Vector3f& max)
	:min_pos(min),max_pos(max) {
		left_node = nullptr;
		right_node = nullptr;
		parent_node = nullptr;
	}
	bool is_intersect(const Ray& ray);
	void split_BVH();
	void resize();
	void clear() { std::vector <Triangle*>().swap(triangle_list); }
};
