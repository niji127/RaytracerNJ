#include "triangle.h"
#include "global.h"
Intersect Triangle::get_intersect(const Ray& ray) {
    Intersect inter;

    if (ray.direction.dot(normal) > EPS)
        return inter;
    double u, v, t_tmp = 0;
    Eigen::Vector3f pvec = ray.direction.cross(e2);
    double det = e1.dot(pvec);
    if (fabs(det) == 0.0)
        return inter;

    double det_inv = 1. / det;
    Eigen::Vector3f tvec = ray.origin - point[0];
    u = tvec.dot(pvec) * det_inv;
    Eigen::Vector3f qvec = tvec.cross(e1);
    v = ray.direction.dot(qvec) * det_inv;
    t_tmp = e2.dot(qvec) * det_inv;

    // TODO find ray triangle intersection
    if (t_tmp < -EPS || u < -EPS || v < -EPS || (1 - u - v) < -EPS)
        return inter;

    inter.dis = t_tmp;
    inter.happened = true;
    inter.material = this->material;
    inter.normal = this->normal;
    inter.xyz = ray.origin + t_tmp * ray.direction;

    return inter;
}
void Triangle::sample(Intersect& inter) {
    inter.normal = this->normal;
    inter.material = this->material;
    inter.happened = true;
    inter.dis = 0.0;
    float cx = get_random_float();
    float cy = get_random_float();
    while (1-cx-cy<0.0f) {
        cx = get_random_float();
        cy = get_random_float();
    }
    inter.xyz = cx * point[0] + cy * point[1] + (1 - cx - cy) * point[2];
}