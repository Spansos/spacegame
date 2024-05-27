#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    // position of the camera
    glm::vec3 position;
    // a vector describing the direction the camera is facing
    glm::vec3 direction;
    // a float describing the roll of the camera, in degrees
    float roll;
    // field of view
    float field_of_view;
    // aspect ratio of the target window.
    // should probs be set up to change on a callback with WindowEventListener
    float aspect_ratio;
    // near clipping plane. things closer than this arent rendered
    float near_plane;
    // near clipping plane. things further than this arent rendered
    float far_plane;
    // calculate a mvp for use in rendering
    glm::mat4 calc_mvp( glm::mat4 model_matrix = glm::mat4{1} ) const;
};