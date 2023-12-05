#include "Mesh.hpp"

#include "util.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace NPR {
Mesh load_mesh_from_obj(const std::string &file_name) {
    std::fstream file("assets/obj/" + file_name);
    if (!file) {
        std::cerr << "Failed to open file: " << file_name << std::endl;
        exit(1);
    }
    std::string line;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<glm::vec2> tex_coords;

    MeshGroup current_group;
    Mesh mesh;

    while (std::getline(file, line)) {
        if (line[0] == '#' || line.empty()) {
            continue;
        }
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") {
            glm::vec3 v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (type == "vn") {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        } else if (type == "vt") {
            glm::vec2 t;
            ss >> t.x >> t.y;
            tex_coords.push_back(t);
        } else if (type == "f") {
            for (int i = 0; i < 3; i++) {
                std::string str;
                ss >> str;
                unsigned int idx;
                if (str.find('/') == std::string::npos) {
                    idx = std::stoul(str);
                } else {
                    idx = std::stoul(util::split_str(str, '/').at(0));
                }
                indices.push_back(idx - 1);
            }
        } else if (type == "g") {
            if (!current_group.name.empty()) {
                current_group.num_indices =
                    indices.size() - current_group.start_face_index;
                mesh.groups.push_back(std::move(current_group));
            }
            ss >> current_group.name;
            if (indices.empty()) {
                current_group.start_face_index = 0;
            } else {
                current_group.start_face_index = indices.size();
            }
        } else if (type == "usemtl") {
            ss >> current_group.material_name;
        } else if (type == "mtllib") {
            // TODO
            throw std::runtime_error("mtllib not implemented");
        } else if (type == "o" || type == "s") {
            // skip command
        } else {
            throw std::runtime_error("Unknown command: " + type);
        }
    }

    if (!current_group.name.empty()) {
        current_group.num_indices =
            indices.size() - current_group.start_face_index;
        mesh.groups.push_back(std::move(current_group));
    }
    current_group.num_indices = indices.size() - current_group.start_face_index;
    mesh.groups.push_back(current_group);

    mesh.vertices = std::make_unique<std::vector<glm::vec3>>(vertices);
    mesh.normals = std::make_unique<std::vector<glm::vec3>>(normals);
    mesh.indices = std::make_unique<std::vector<unsigned int>>(indices);
    mesh.tex_coords = std::make_unique<std::vector<glm::vec2>>(tex_coords);

    // TODO: associate materials
    for (auto &g : mesh.groups) {
        // g.material = ?
    }

    return mesh;
}

} // namespace NPR
