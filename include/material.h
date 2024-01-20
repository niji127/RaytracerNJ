#pragma once
#include <Eigen/Dense>
enum class MaterialType { DIFFUSE, REFLECT, LIGHT, TRANSPARENT};

class Material {
public:
    MaterialType m_type;
    Eigen::Vector3f m_color;
    Eigen::Vector3f m_emission;

    Material(MaterialType t = MaterialType::DIFFUSE,
        Eigen::Vector3f c = { 1.0f, 1.0f, 1.0f }) :
        m_type(t){
        if (t == MaterialType::DIFFUSE || t == MaterialType::REFLECT || t == MaterialType::TRANSPARENT) {
            this->m_color = c;
            this->m_emission = { 0.0f, 0.0f, 0.0f };
        }
        else if (t == MaterialType::LIGHT) {
            this->m_color= { 0.0f, 0.0f, 0.0f };
            this->m_emission = c;
        }
    };
};