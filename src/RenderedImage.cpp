#include "RenderedImage.hpp"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

void RenderedImage::DrawEdges(float edge_strength) {
    const auto beta_edges = diffuse_.GetEdges();

    // Multiply to get edged image
    const auto multiply_inverse = [edge_strength](glm::vec3 a, glm::vec3 b) {
        return a * (glm::vec3(1.0, 1.0, 1.0) - edge_strength * b);
    };
    
    final_ = color_.ApplyLayer(beta_edges, multiply_inverse);
}