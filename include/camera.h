#pragma once
#include "light.h"
#include "global.h"
#include <Eigen/dense>

class Camera {
public:
	Eigen::Vector3f view_point;
	int width;
	int height;
	float aspect_ratio;
	float fov;


	Camera(const Eigen::Vector3f& p, int w,int h,float f):view_point(p),width(w),height(h),fov(f){
		aspect_ratio = (float)width / (float)height;
	}

	Ray cast_ray(const Eigen::Vector2f& p);
};



