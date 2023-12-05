#ifndef MESH
#define MESH

#include "Material.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace NPR {
struct MeshGroup {
    std::string name;
    size_t start_face_index;
    size_t num_indices;
    std::string material_name;
    std::shared_ptr<Material> material;
};

struct Mesh {
    std::unique_ptr<std::vector<glm::vec3>> vertices;
    std::unique_ptr<std::vector<glm::vec3>> normals;
    std::unique_ptr<std::vector<unsigned int>> indices;
    std::unique_ptr<std::vector<glm::vec2>> tex_coords;

    std::vector<MeshGroup> groups;
};

/**
 * Loads a mesh from an OBJ file.
 *
 * @param file_name The name of the OBJ file to load.
 *
 * @return A Mesh object representing the loaded mesh.
 *
 * @throws std::runtime_error if the file fails to open or if an unknown
 *         command is encountered.
 */
Mesh load_mesh_from_obj(const std::string &file_name);
} // namespace NPR

#endif