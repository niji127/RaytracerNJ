#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <Eigen/Dense>
#include "intersect.h"
#include "renderer.h"
#include "camera.h"
#include "scene.h"
#include "BVH.h"

int main() {
    Intersect inter1;
    Intersect inter2;
    inter2.happened = true;
    inter1 = inter2;
    Scene scene;

    Material red(MaterialType::DIFFUSE, { 0.63f, 0.065f, 0.05f });
    Material green(MaterialType::DIFFUSE, { 0.14f, 0.45f, 0.091f });
    Material blue(MaterialType::DIFFUSE, { 19.0f/255.0f, 103.0f / 255.0f, 131.0f / 255.0f });
    Material orange(MaterialType::DIFFUSE, { 250.0f / 255.0f, 134.0f / 255.0f, 0.0f / 255.0f });
    Material white(MaterialType::DIFFUSE, { 0.725f, 0.71f, 0.68f });
    Material reflect(MaterialType::REFLECT, { 0.725f, 0.71f, 0.68f });
    Material transparent(MaterialType::TRANSPARENT, { 0.725f, 0.71f, 0.68f });
    Material white1(MaterialType::DIFFUSE, { 0.725f, 1.0f, 0.69f });
    Material white2(MaterialType::DIFFUSE, { 0.725f, 1.0f, 0.7f });

    Eigen::Vector3f light_vector;
    light_vector.x() = 8.0f * (0.747f + 0.058f) + 15.6f * (0.740f + 0.287f) + 18.4f * (0.737f + 0.642f);
    light_vector.y() = 8.0f * (0.747f + 0.258f) + 15.6f * (0.740f + 0.160f) + 18.4f * (0.737f + 0.159f);
    light_vector.z() = 8.0f * 0.747f + 15.6f * 0.740f + 18.4f * 0.737f;
    Material illumination(MaterialType::LIGHT, light_vector);
    
    Object left("../model/left.obj", blue);
    Object right("../model/right.obj", orange);
    Object floor("../model/floor.obj", white);
    Object shortbox("../model/shortbox.obj", white);
    Object tallbox("../model/tallbox.obj", reflect);
    Object light("../model/light.obj", illumination);
    

    scene.add(&left);
    scene.add(&right);
    scene.add(&floor);
    scene.add(&shortbox);
    scene.add(&tallbox);
    scene.add(&light);
    
    BVH* bvh_root = new BVH(scene);
    bvh_root->split_BVH();

    Eigen::Vector3f camera_pos = { 278.0f, 273.0f, -800.0f };
    Camera camera(camera_pos, 1000, 1000, 40.0f);

    Renderer r(&camera, bvh_root, &scene);
    r.render();
    //render();
    return 0;
}
