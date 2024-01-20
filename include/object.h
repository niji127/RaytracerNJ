#pragma once
#include <vector>
#include <Eigen/Dense>
#include "light.h"
#include "material.h"
#include "triangle.h"



class Object {
public:
    Object(const std::string& filename, const Material& m);
    Material material;
    int triangle_num;
    float area;
    Eigen::Vector3f min_pos;
    Eigen::Vector3f max_pos;
    std::vector<Triangle*> triangle_list;
    
};

//class Sphere :public Object {
//public:
//    Eigen::Vector3f center;
//    float radius;
//
//    Sphere(const Eigen::Vector3f& c, const float& r) : center(c), radius(r) {};
//
//    bool ray_intersect(const Ray&ray,const Sphere&sphere) const {
//        Eigen::Vector3f dis = sphere.center - ray.origin;
//        // 光源位于球内
//        auto dis_norm = dis.norm();
//        if (dis_norm < sphere.radius) return true;
//        // 球在光源后
//        auto dis_proj = dis.dot(ray.direction);
//        if (dis_proj < 0.0) return false;
//        // 投影三角形
//        float d2 = dis_norm * dis_norm - dis_proj * dis_proj;
//        if (d2 <= radius * radius) return true;
//        return false;
//    }
//};
//
//
//
//class Plane :public Object {
//public:
//    std::vector<Eigen::Vector3f> point[4];
//
//};
//
//class Cuboid :public Object {
//
//};