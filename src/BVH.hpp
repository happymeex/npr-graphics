#ifndef BVH
#define BVH

#include "HitRecord.hpp"
#include "Ray.hpp"
#include "Shapes.hpp"
#include <vector>

typedef unsigned int uint;
struct BVHNode {
    glm::vec3 min, max;
    uint left_idx, num_triangles;
};
class BVHObject {
  public:
    BVHObject(std::vector<Triangle> &triangles)
        : nodes_created(0), triangles_(triangles){};
    void ConstructBVH();
    bool Intersects(const Ray &ray, float t_min, HitRecord &record,
                    uint node_id = 0);

  private:
    uint nodes_created;
    std::vector<BVHNode> nodes_;
    std::vector<Triangle> &triangles_;
    std::vector<glm::vec3> centroids_;
    /**
     * Tracks whether a node in the BVH is a leaf or not.
     */
    std::vector<bool> is_leaf_;
    /**
     * Reordering of the triangles: triangle_indices_[i] returns an index
     * of the i-th triangle in the BVH, namely an index for a triangle in
     * `triangles_`.
     *
     * The fields `left_idx` and `num_triangles` of the BVH nodes are
     * used to index into `triangle_indices_`.
     */
    std::vector<uint> triangle_indices_;
    /**
     * Sets the bounding box of a node in the BVH based on the triangles
     * currently assigned to the node.
     */
    void SetNodeBoundingBox(uint node_id);
    /**
     * Recursively splits a node in the BVH.
     */
    void SplitNode(uint node_id);
    /**
     * Checks if a ray intersects the bounding box of a node in the BVH.
     * Does not mutate the hit record.
     */
    bool IntersectsBoundingBox(const Ray &ray, float t_min,
                               const HitRecord &record, const BVHNode &node);
};

#endif