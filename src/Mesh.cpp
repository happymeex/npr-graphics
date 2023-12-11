#include "Mesh.hpp"

#include "Shapes.hpp"
#include "util.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

void Mesh::UpdateTriangles() {
    triangles.clear();
    for (size_t i = 0; i < indices->size(); i += 3) {
        int j1 = indices->at(i);
        int j2 = indices->at(i + 1);
        int j3 = indices->at(i + 2);
        auto v1 = vertices->at(j1);
        auto v2 = vertices->at(j2);
        auto v3 = vertices->at(j3);
        Triangle triangle = Triangle(v1, v2, v3);
        if (j1 < (int)normals->size() && j2 < (int)normals->size() &&
            j3 < (int)normals->size()) {
            triangle = Triangle(v1, v2, v3, normals->at(j1), normals->at(j2),
                                normals->at(j3));
        }
        triangles.push_back(triangle);
    }
}

void Mesh::UpdateNormals() {
    normals->clear();
    normals->resize(vertices->size(), glm::vec3(0.f, 0.f, 0.f));
    for (size_t i = 0; i < indices->size(); i += 3) {
        int v1 = indices->at(i);
        int v2 = indices->at(i + 1);
        int v3 = indices->at(i + 2);
        auto &p1 = vertices->at(v1);
        auto &p2 = vertices->at(v2);
        auto &p3 = vertices->at(v3);
        glm::vec3 n = glm::cross(p2 - p1, p3 - p1);
        normals->at(v1) += n;
        normals->at(v2) += n;
        normals->at(v3) += n;
    }

    for (auto &n : *normals) {
        n = glm::normalize(n);
    }
}

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

    std::unordered_map<unsigned int, unsigned int> vtx_to_normal;

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
                    auto split = util::split_str(str, '/');
                    idx = std::stoul(split[0]);
                    if (split.size() == 3) {
                        vtx_to_normal[idx - 1] = std::stoul(split[2]) - 1;
                    }
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

    mesh.vertices = std::make_unique<std::vector<glm::vec3>>(vertices);
    mesh.normals = std::make_unique<std::vector<glm::vec3>>(normals);
    mesh.indices = std::make_unique<std::vector<unsigned int>>(indices);
    mesh.tex_coords = std::make_unique<std::vector<glm::vec2>>(tex_coords);

    if (mesh.normals->empty()) {
        // no normals provided, compute them based on vertices and faces
        mesh.UpdateNormals();
    } else {
        // all normals provided
        mesh.AssociateNormals(vtx_to_normal);
    }
    mesh.UpdateTriangles();

    // TODO: associate materials
    for (auto &g : mesh.groups) {
        // g.material = ?
    }

    return mesh;
}

void Mesh::AssociateNormals(
    const std::unordered_map<unsigned int, unsigned int> &vtx_to_normal) {
    std::vector<glm::vec3> new_normals;
    for (unsigned int i = 0; i < (unsigned int)vertices->size(); i++) {
        auto it = vtx_to_normal.find(i);
        if (it != vtx_to_normal.end()) {
            new_normals.push_back(normals->at(it->second));
        } else {
            new_normals.push_back({0, 0, 0});
        }
    }
    normals = std::make_unique<std::vector<glm::vec3>>(new_normals);
}