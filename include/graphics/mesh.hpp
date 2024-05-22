#pragma once

#include <glm/glm.hpp>
#include <unordered_set>
#include <vector>

struct Mesh {
    Mesh & operator+=( const Mesh & other );
    Mesh operator+( const Mesh & other );
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;
};