#pragma once
#include <Eigen/Dense>
#include <iostream>
#include "material.h"
#include "light.h"
#include "intersect.h"
class Triangle {
public:
    std::vector<Eigen::Vector3f> point;
    Eigen::Vector3f e1, e2;
    Eigen::Vector3f normal;
    Eigen::Vector3f center;
    //std::vector<Eigen::Vector3f> normal[3];
    //std::vector<Eigen::Vector2f> uv[3];
    Material material;
    float area;
    float object_area = 0.0f;

    Triangle(const std::vector<Eigen::Vector3f>& p, const Material& mt)
        :point(p), material(mt) {
        e1 = point[1] - point[0];
        e2 = point[2] - point[0];
        this->normal = e1.cross(e2);
        this->area = this->normal.norm() * 0.5f;
        this->normal.normalize();
        this->center = 1.0f/3.0f * (point[0] + point[1] + point[2]);
    }
    Intersect get_intersect(const Ray& ray);
    void sample(Intersect& inter);

};