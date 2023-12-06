#include "BVH.hpp"
#include <iostream>

bool BVHObject::Intersects(const Ray &ray, float t_min, HitRecord &record,
                           uint node_id) {
    bool hit = false;
    auto &node = nodes_.at(node_id);
    if (!IntersectsBoundingBox(ray, t_min, record, node)) {
        return false;
    }
    if (is_leaf_.at(node_id)) {
        for (uint i = node.left_idx; i < node.left_idx + node.num_triangles;
             i++) {
            hit |= triangles_.at(triangle_indices_.at(i))
                       .Intersects(ray, t_min, record);
        }
    } else {
        hit |= Intersects(ray, t_min, record, node.left_idx);
        hit |= Intersects(ray, t_min, record, node.left_idx + 1);
    }
    return hit;
}

void BVHObject::ConstructBVH() {
    uint N = triangles_.size();
    uint MAX_NODES = 2 * N - 1;
    is_leaf_.resize(MAX_NODES, false);
    // compute centroids
    for (uint i = 0; i < N; i++) {
        auto positions = triangles_.at(i).GetPositions();
        centroids_.push_back((positions[0] + positions[1] + positions[2]) /
                             3.0f);
    }
    for (uint i = 0; i < N; i++) {
        triangle_indices_.push_back(i);
    }
    nodes_.resize(MAX_NODES);
    auto &root = nodes_.at(0);
    root.left_idx = 0;
    root.num_triangles = N;
    nodes_created = 1;
    SetNodeBoundingBox(0);
    SplitNode(0);
}

void BVHObject::SplitNode(uint node_id) {
    auto &node = nodes_.at(node_id);
    if (node.num_triangles <= 2) {
        // mark as leaf
        is_leaf_.at(node_id) = true;
        return;
    }
    // find longest axis of current bounding box
    int axis = 0;
    glm::vec3 range = node.max - node.min;
    if (range.y > range.x)
        axis = 1;
    if (range.z > range[axis])
        axis = 2;
    // Reorder triangles into left and right nodes
    float cutoff = (node.min[axis] + node.max[axis]) / 2.0f;
    uint i = node.left_idx;
    uint j = node.left_idx + node.num_triangles;
    while (i < j) {
        if (centroids_.at(triangle_indices_.at(i))[axis] < cutoff) {
            i++;
        } else {
            std::swap(triangle_indices_.at(i), triangle_indices_.at(j - 1));
            j--;
        }
    }
    int num_triangles_left = i - node.left_idx;
    if (num_triangles_left == 0 ||
        num_triangles_left == (int)node.num_triangles) {
        // mark as leaf
        is_leaf_.at(node_id) = true;
        return;
    }
    // Create children
    uint left_node_idx = nodes_created;      // index of left child in `nodes_`
    uint right_node_idx = nodes_created + 1; // index of right child in `nodes_`
    nodes_created += 2;
    nodes_[left_node_idx].left_idx = node.left_idx;
    nodes_[left_node_idx].num_triangles = num_triangles_left;
    nodes_[right_node_idx].left_idx = i;
    nodes_[right_node_idx].num_triangles =
        node.num_triangles - num_triangles_left;
    node.num_triangles = 0; // indicates non-leaf
    node.left_idx = left_node_idx;
    SetNodeBoundingBox(left_node_idx);
    SetNodeBoundingBox(right_node_idx);

    SplitNode(left_node_idx);
    SplitNode(right_node_idx);
}

void BVHObject::SetNodeBoundingBox(uint node_id) {
    auto &node = nodes_.at(node_id);
    node.min = glm::vec3(std::numeric_limits<float>::max());
    node.max = glm::vec3(-std::numeric_limits<float>::max());
    for (uint i = node.left_idx; i < node.left_idx + node.num_triangles; i++) {
        auto positions = triangles_.at(triangle_indices_.at(i)).GetPositions();
        node.min = glm::min(node.min, positions[0]);
        node.min = glm::min(node.min, positions[1]);
        node.min = glm::min(node.min, positions[2]);
        node.max = glm::max(node.max, positions[0]);
        node.max = glm::max(node.max, positions[1]);
        node.max = glm::max(node.max, positions[2]);
    }
}

bool BVHObject::IntersectsBoundingBox(const Ray &ray, float t_min,
                                      const HitRecord &record,
                                      const BVHNode &node) {
    float tx1 = (node.min.x - ray.origin.x) / ray.direction.x;
    float tx2 = (node.max.x - ray.origin.x) / ray.direction.x;
    float ty1 = (node.min.y - ray.origin.y) / ray.direction.y;
    float ty2 = (node.max.y - ray.origin.y) / ray.direction.y;
    float tz1 = (node.min.z - ray.origin.z) / ray.direction.z;
    float tz2 = (node.max.z - ray.origin.z) / ray.direction.z;

    // latest time among the "first-hits" in each axis
    float early = std::min(tx1, tx2);
    early = std::max(early, std::min(ty1, ty2));
    early = std::max(early, std::min(tz1, tz2));
    // earliest time among the "last-hits" in each axis
    float late = std::max(tx1, tx2);
    late = std::min(late, std::max(ty1, ty2));
    late = std::min(late, std::max(tz1, tz2));

    return early <= late && early <= record.time && late >= t_min;
}