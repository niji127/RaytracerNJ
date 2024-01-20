#include <iostream>
#include "BVH.h"
#include "global.h"

void BVH::split_BVH() {
	// 确定划分维度
	int dim;
	int size = triangle_list.size();
	Eigen::Vector3f dim_length = max_pos - min_pos;
	if (dim_length.x() > dim_length.y() && dim_length.x() > dim_length.z())
		dim = 0;
	else if (dim_length.y() > dim_length.z())
		dim = 1;
	else
		dim = 2;

	// 取重心坐标中位数
	std::vector<float> coord_list;
	coord_list.reserve(size);
	switch (dim) {
	case 0:
		for (auto iter : triangle_list) {
			coord_list.push_back(iter->center.x());
		}
		break;
	case 1:
		for (auto iter : triangle_list) {
			coord_list.push_back(iter->center.y());
		}
		break;
	case 2:
		for (auto iter : triangle_list) {
			coord_list.push_back(iter->center.z());
		}
		break;
	}
	std::sort(coord_list.begin(), coord_list.end());
	float split = coord_list[(int)(size / 2)];
	if (split == coord_list[0]) split += 0.01;

	// 生成子块
	left_node = new BVH(min_pos, max_pos);
	right_node = new BVH(min_pos, max_pos);
	left_node->parent_node = this;
	right_node->parent_node = this;
	switch (dim) {
	case 0:
		for (int i = 0; i < size;i++) {
			if (this->triangle_list[i]->center.x() < split)
				left_node->triangle_list.push_back(this->triangle_list[i]);
			else
				right_node->triangle_list.push_back(this->triangle_list[i]);
		}
		break;
	case 1:
		for (int i = 0; i < size; i++) {
			if (this->triangle_list[i]->center.y() < split)
				left_node->triangle_list.push_back(this->triangle_list[i]);
			else
				right_node->triangle_list.push_back(this->triangle_list[i]);
		}
		break;
	case 2:
		for (int i = 0; i < size; i++) {
			if (this->triangle_list[i]->center.z() < split)
				left_node->triangle_list.push_back(this->triangle_list[i]);
			else
				right_node->triangle_list.push_back(this->triangle_list[i]);
		}
		break;
	}

	left_node->resize();
	right_node->resize();
	this->clear();
	if (left_node->triangle_list.size() > BVH_MAX_TRI)
		left_node->split_BVH();
	if (right_node->triangle_list.size() > BVH_MAX_TRI)
		right_node->split_BVH();

}

void BVH::resize() {
	Eigen::Vector3f min_xyz = { 1e6,1e6,1e6 };
	Eigen::Vector3f max_xyz = { -1e6,-1e6,-1e6 };
	for (auto iter : triangle_list) {
		for (int i = 0; i < 3; i++) {
				min_xyz.x() = std::min(min_xyz.x(), iter->point[i].x());
				min_xyz.y() = std::min(min_xyz.y(), iter->point[i].y());
				min_xyz.z() = std::min(min_xyz.z(), iter->point[i].z());

				max_xyz.x() = std::max(max_xyz.x(), iter->point[i].x());
				max_xyz.y() = std::max(max_xyz.y(), iter->point[i].y());
				max_xyz.z() = std::max(max_xyz.z(), iter->point[i].z());
		}
	}
	max_pos = max_xyz;
	min_pos = min_xyz;
}

bool BVH::is_intersect(const Ray& ray) {
	Eigen::Vector3f tmin = (min_pos - ray.origin).cwiseProduct(ray.direction_inv);
	Eigen::Vector3f tmax = (max_pos - ray.origin).cwiseProduct(ray.direction_inv);

	if (ray.direction.x() < 0.0f)
		std::swap(tmin.x(),tmax.x());
	if (ray.direction.y() < 0.0f)
		std::swap(tmin.y(), tmax.y());
	if (ray.direction.z() < 0.0f)
		std::swap(tmin.z(), tmax.z());

	float t_enter = tmin.maxCoeff();
	float t_exit = tmax.minCoeff();

		if (std::isinf(tmin.x())) {
			if (ray.origin.x() > max_pos.x() + EPS || ray.origin.x() < min_pos.x() - EPS)
				return false;
		}
		if (std::isinf(tmin.y())) {
			if (ray.origin.y() > max_pos.y() + EPS || ray.origin.y() < min_pos.x() - EPS)
				return false;
		}
		if (std::isinf(tmin.z())) {
			if (ray.origin.z() > max_pos.z() + EPS || ray.origin.z() < min_pos.z() - EPS)
				return false;
		}
		if (std::isinf(tmin.x())) {
			if (std::isinf(tmin.y())) {
				t_enter = tmin.z();
				t_exit = tmax.z();
			}
			else if (std::isinf(tmin.z())) {
				t_enter = tmin.y();
				t_exit = tmax.y();
			}
			else {
				t_enter = std::max(tmin.y(), tmin.z());
				t_exit = std::min(tmax.y(), tmax.z());
			}
		}
		else if (std::isinf(tmin.y())) {
			if (std::isinf(tmin.z())) {
				t_enter = tmin.x();
				t_exit = tmax.x();
			}
			else {
				t_enter = std::max(tmin.x(), tmin.z());
				t_exit = std::min(tmax.x(), tmax.z());
			}
		}
		else if (std::isinf(tmin.z())) {
			t_enter = std::max(tmin.x(), tmin.y());
			t_exit = std::min(tmax.x(), tmax.y());
		}

		if (t_enter > t_exit + EPS || t_exit < -EPS)
			return false;
		else
			return true;
}
