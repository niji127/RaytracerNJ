#include "object.h"
#include "OBJ_Loader.h"

Object::Object(const std::string& filename, const Material& m) :material(m)
{
    objl::Loader loader;
    loader.LoadFile(filename);
    area = 0.0;
    assert(loader.LoadedMeshes.size() == 1);
    objl::Mesh mesh = loader.LoadedMeshes[0];
    area = 0.0f;
    min_pos = { 1.0e6,1.0e6,1.0e6 };
    max_pos = { -1.0e6,-1.0e6,-1.0e6 };


    for (int i = 0; i < mesh.Vertices.size(); i += 3) {
        std::vector<Eigen::Vector3f> face_vertices(3);

        for (int j = 0; j < 3; j++) {
            Eigen::Vector3f vert = { mesh.Vertices[i + j].Position.X,
                mesh.Vertices[i + j].Position.Y,
                mesh.Vertices[i + j].Position.Z };

            min_pos = { std::min(min_pos.x(),vert.x()),
                std::min(min_pos.y(),vert.y()),
                std::min(min_pos.z(),vert.z()) };
            max_pos = { std::max(max_pos.x(),vert.x()),
                std::max(max_pos.y(),vert.y()),
                std::max(max_pos.z(),vert.z()) };

            face_vertices[j] = vert;
        }
        Triangle* t = new Triangle(face_vertices, material);
        this->triangle_list.push_back(t);
        area += t->area;
    }
    triangle_num = (int)triangle_list.size();
    for (auto iter : triangle_list) {
        iter->object_area = area;
    }
}

