#include "graphics/mesh.hpp"

Mesh & Mesh::operator+=( const Mesh & other ) {
    this->positions.insert( this->positions.end(), other.positions.begin(), other.positions.end() );
    this->normals.insert( this->normals.end(), other.normals.begin(), other.normals.end() );
    this->colors.insert( this->colors.end(), other.colors.begin(), other.colors.end() );
    return *this;
}

Mesh Mesh::operator+( const Mesh & other ) {
    Mesh r = *this;
    r += other;
    return r;
}