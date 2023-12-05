#ifndef MESH
#define MESH

#include "Material.hpp"
#include "Shapes.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

struct MeshGroup {
    std::string name;
    size_t start_face_index;
    size_t num_indices;
    std::string material_name;
    std::shared_ptr<Material> material;
};

class Mesh {
  public:
    std::unique_ptr<std::vector<glm::vec3>> vertices;
    std::unique_ptr<std::vector<glm::vec3>> normals;
    std::unique_ptr<std::vector<unsigned int>> indices;
    std::unique_ptr<std::vector<glm::vec2>> tex_coords;
    /**
     * Updates the triangles of the mesh according to current values in
     * `vertices`, `indices`, and `normals`.
     */
    void UpdateTriangles();
    /**
     * Computes the normals of the mesh by using the weighted average of the
     * normals of triangles incident on each vertex.
     * Does not depend on `triangles` being updated, only `vertices` and
     * `indices`.
     */
    void UpdateNormals();

    std::vector<MeshGroup> groups;
    std::vector<Triangle> triangles;
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

#endif