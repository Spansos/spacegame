#include "world/world.hpp"

#include <limits>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/intersect.hpp>


Chunk::Chunk( World * world, glm::ivec3 chunk_pos ) : _voxels( {0} ), _world(world), _chunk_pos(chunk_pos) {}

void Chunk::set_voxel( glm::ivec3 position, float value ) {
    get_voxel(position) = value;
}

glm::vec3 VertexInterp(glm::vec3 p1, glm::vec3 p2, float valp1, float valp2)
{
    double mu;
    glm::vec3 p;

    // if (std::abs(ISO_LEVEL-valp1) < 0.00001)
    //     return(p1);
    // if (std::abs(ISO_LEVEL-valp2) < 0.00001)
    //     return(p2);
    // if (std::abs(valp1-valp2) < 0.00001)
    //     return(p1);
    mu = (ISO_LEVEL - valp1) / (valp2 - valp1);
    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);

    return(p);
}

void Chunk::cube_march() {
    int edge_table[256] =
        #include "world/edge_table.incl"
    int tri_table[256][16] =
        #include "world/tri_table.incl"

    _triangles.clear();
    for ( int x=0; x<CHUNK_SIZE; ++x ) {
        for ( int y=0; y<CHUNK_SIZE; ++y ) {
            for ( int z=0; z<CHUNK_SIZE; ++z ) {
                // calc pos
                glm::ivec3 pos = _chunk_pos * CHUNK_SIZE + glm::ivec3{x,y,z};
                // positions of all 8 voxels in cube
                std::array<glm::ivec3, 8> voxels = {
                    pos+glm::ivec3{0,0,0},
                    pos+glm::ivec3{1,0,0},
                    pos+glm::ivec3{1,0,1},
                    pos+glm::ivec3{0,0,1},
                    pos+glm::ivec3{0,1,0},
                    pos+glm::ivec3{1,1,0},
                    pos+glm::ivec3{1,1,1},
                    pos+glm::ivec3{0,1,1}
                };
                // values of those voxels
                std::array<float, 8> values;
                for ( int i=0; i<8; i++ )
                    values[i] = _world->get_voxel(voxels[i]).value_or(ISO_LEVEL);
                // make cube index
                int cubeindex = 0;
                for ( int i=0; i<8; ++i )
                    cubeindex |= (values[i] > ISO_LEVEL) << i;

                // get edges mask
                int edges = edge_table[cubeindex];
                if ( edges == 0 )
                    continue;

                std::array<glm::vec3, 12> vertlist;
                if (edges & 1)
                    vertlist[0] =
                        VertexInterp( voxels[0], voxels[1], values[0], values[1]);
                if (edges & 2)
                    vertlist[1] =
                        VertexInterp( voxels[1], voxels[2], values[1], values[2]);
                if (edges & 4)
                    vertlist[2] =
                        VertexInterp( voxels[2], voxels[3], values[2], values[3]);
                if (edges & 8)
                    vertlist[3] =
                        VertexInterp( voxels[3], voxels[0], values[3], values[0]);
                if (edges & 16)
                    vertlist[4] =
                        VertexInterp( voxels[4], voxels[5], values[4], values[5]);
                if (edges & 32)
                    vertlist[5] =
                        VertexInterp( voxels[5], voxels[6], values[5], values[6]);
                if (edges & 64)
                    vertlist[6] =
                        VertexInterp( voxels[6], voxels[7], values[6], values[7]);
                if (edges & 128)
                    vertlist[7] =
                        VertexInterp( voxels[7], voxels[4], values[7], values[4]);
                if (edges & 256)
                    vertlist[8] =
                        VertexInterp( voxels[0], voxels[4], values[0], values[4]);
                if (edges & 512)
                    vertlist[9] =
                        VertexInterp( voxels[1], voxels[5], values[1], values[5]);
                if (edges & 1024)
                    vertlist[10] =
                        VertexInterp( voxels[2], voxels[6], values[2], values[6]);
                if (edges & 2048)
                    vertlist[11] =
                        VertexInterp( voxels[3], voxels[7], values[3], values[7]);

                for ( int i=0;tri_table[cubeindex][i]!=-1;i+=3 ) {
                    Triangle triangle;
                    triangle[0] = vertlist[tri_table[cubeindex][i]];
                    triangle[1] = vertlist[tri_table[cubeindex][i+1]];
                    triangle[2] = vertlist[tri_table[cubeindex][i+2]];
                    _triangles.push_back( triangle );
                }
            }
        }
    }
}

Mesh Chunk::mesh() {
    Mesh mesh;
    auto begin = reinterpret_cast<glm::vec3*>(_triangles.data());
    mesh.positions.insert( mesh.positions.end(), begin, begin+_triangles.size()*3 );
    for ( auto t : _triangles ) {
        for ( int i=0; i<3; i++ ) {
            mesh.colors.push_back( { .9, .2, .3 } );
            mesh.normals.push_back( glm::triangleNormal( t[0], t[1], t[2] ) );
        }
    }
    return mesh;
}

std::optional<glm::vec3> Chunk::raycast( glm::vec3 start, glm::vec3 dir ) const {
    std::pair<glm::vec3, float> result = {{}, std::numeric_limits<float>::infinity()};
    for ( const auto & triangle : _triangles ) {
        glm::vec2 bary;
        float dist;
        if ( glm::intersectRayTriangle( start, dir, triangle[0], triangle[1], triangle[2], bary, dist ) ) {
            if ( dist < result.second ) {
                result.first = bary.x * triangle[0]
                    + bary.y * triangle[1]
                    + (1-bary.x-bary.y) * triangle[2];
                result.second = dist;
            }
        }
    }
    if ( result.second == std::numeric_limits<float>::infinity() )
        return {};
    return result.first;
}

float & Chunk::get_voxel( glm::ivec3 position ) {
    return _voxels[ position.x + position.y*CHUNK_SIZE + position.z*CHUNK_SIZE*CHUNK_SIZE ];
}

World::World() {
    for ( int x=-20; x<=20; ++x ) {
        for ( int y=-20; y<=20; ++y ) {
            for ( int z=-20; z<=20; ++z ) {
                this->set_voxel( {x,y,z}, 16.0/glm::length( glm::vec3{x,y,z} )-1 );
            }
        }
    }
    for ( auto &i : _chunks ) {
        i.second.cube_march();
    }
}

void World::set_voxel( glm::ivec3 position, float value ) {
    auto local_pos = calc_chunk_coordinates( position );

    if ( _chunks.find(local_pos.first) == _chunks.end() ) {
        _chunks.insert( { local_pos.first, Chunk( this, local_pos.first ) } );
    }

    _chunks.at(local_pos.first).set_voxel( local_pos.second, value );
}

std::optional<float> World::get_voxel( glm::ivec3 position ) {
    auto local_pos = calc_chunk_coordinates( position );
    auto it = _chunks.find(local_pos.first);
    if ( it == _chunks.end() )
        return {};
    return it->second.get_voxel(local_pos.second);
}

Chunk & World::get_chunk( glm::ivec3 chunk_position ) {
    auto it = _chunks.find( chunk_position );
    if ( it == _chunks.end() ) {
        _chunks.insert( {chunk_position, {this, chunk_position}} );
        return _chunks.at( chunk_position );
    }
    return it->second;
}

std::pair<glm::ivec3, glm::ivec3> World::calc_chunk_coordinates( glm::ivec3 world_coordinates ) {
    glm::ivec3 chunk_pos = world_coordinates / CHUNK_SIZE;
    chunk_pos -= glm::notEqual(chunk_pos*CHUNK_SIZE, world_coordinates) * glm::lessThan( world_coordinates, {0,0,0} );
    glm::ivec3 block_pos = world_coordinates - chunk_pos*CHUNK_SIZE;
    return { chunk_pos, block_pos };
}

std::optional<glm::vec3> World::raycast( glm::vec3 start, glm::vec3 dir ) {
    glm::vec3 result = { std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
    for ( const auto & pair : _chunks ) {
        auto r = pair.second.raycast( start, dir );
        if ( r.has_value() && glm::length2(start-r.value()) < glm::length2(start-result) ) {
            result = r.value();
        }
    }
    if ( result.x == std::numeric_limits<float>::infinity() )
        return {};
    return result;
}

void World::render( Window & window, Renderer & renderer, const Camera & camera ) {
    for ( auto &chunk : _chunks ) {
        renderer.draw( window, chunk.second.mesh(), glm::mat4{1}, camera );
    }
}