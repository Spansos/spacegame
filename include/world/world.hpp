#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <array>
#include <utility>
#include <optional>

#include "graphics/mesh.hpp"
#include "graphics/renderer.hpp"

typedef std::array<glm::vec3, 3> Triangle;

const int CHUNK_SIZE = 8;
const float ISO_LEVEL = 0;

class World;

class Chunk {
public:
    Chunk( World * world, glm::ivec3 chunk_pos );
    void set_voxel( glm::ivec3 position, float value );
    void cube_march();
    Mesh mesh();
    std::optional<glm::vec3> raycast( glm::vec3 start, glm::vec3 dir ) const;
// private:
    float & get_voxel( glm::ivec3 position );
    std::array<float, CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE> _voxels;
    std::vector<Triangle> _triangles;
    World * _world;
    glm::ivec3 _chunk_pos;
};

class World {
public:
    World();
    void set_voxel( glm::ivec3 position, float value );
    std::optional<float> get_voxel( glm::ivec3 position );
    Chunk & get_chunk( glm::ivec3 chunk_position );
    std::pair<glm::ivec3, glm::ivec3> calc_chunk_coordinates( glm::ivec3 world_coordinates );
    std::optional<glm::vec3> raycast( glm::vec3 start, glm::vec3 dir );
    void render( Window & window, Renderer & renderer, const Camera & camera );
private:
    std::unordered_map<glm::ivec3, Chunk> _chunks;
};