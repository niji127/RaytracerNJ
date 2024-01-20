#pragma once
#include "camera.h"
#include "BVH.h"
#include "scene.h"
#include "intersect.h"
class Renderer {
public:
	Camera* camera;
	BVH* bvh_root;
	Scene* scene;

	Renderer(Camera* c, BVH* b, Scene* s) :camera(c), bvh_root(b), scene(s) {}
	Intersect get_intersect(const Ray& ray, BVH* bvh);
	Eigen::Vector3f get_color(const Ray& ray,int depth);
	
	void render();
};