#pragma once
#include <random>
#include <Eigen/Dense>
#include <iostream>
#define PI 3.141592653589793f
#define EPS 1e-3f
#define BVH_MAX_TRI 5

inline float get_random_float()
{
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(rng);
}
inline Eigen::Vector3f to_world(const Eigen::Vector3f& a, const Eigen::Vector3f& N) {
    Eigen::Vector3f B, C;
    if (std::fabs(N.x()) > std::fabs(N.y())) {
        float invLen = 1.0f / std::sqrt(N.x() * N.x() + N.z() * N.z());
        C = Eigen::Vector3f(N.z() * invLen, 0.0f, -N.x() * invLen);
    }
    else {
        float invLen = 1.0f / std::sqrt(N.y() * N.y() + N.z() * N.z());
        C = Eigen::Vector3f(0.0f, N.z() * invLen, -N.y() * invLen);
    }
    B = C.cross(N);
    return a.x() * B + a.y() * C + a.z() * N;
}
inline Eigen::Vector3f sample(Eigen::Vector3f N) {
    float x_1 = get_random_float(), x_2 = get_random_float();
    float z = std::fabs(1.0f - 2.0f * x_1);
    float r = std::sqrt(1.0f - z * z), phi = 2 * PI * x_2;
    Eigen::Vector3f localRay(r * std::cos(phi), r * std::sin(phi), z);
    return to_world(localRay, N);
}
inline void UpdateProgress(float progress)
{
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
};