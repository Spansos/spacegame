#include "graphics/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

glm::mat4 Camera::calc_mvp( glm::mat4 model_matrix ) const {
    // perspective
    glm::mat4 mvp = glm::perspective(
        glm::radians(field_of_view),
        aspect_ratio,
        near_plane,
        far_plane
    );

    // view
    // right and up vectors
    // // we use glm::vec3{0,1,0} only because 0 degs should be have y be up
    glm::vec3 right = glm::cross( direction, glm::vec3{0, 1, 0} );
    glm::vec3 up = glm::normalize(glm::cross( right, direction ));
    up = glm::rotate( glm::radians(roll), direction ) * glm::vec4{up,0};
    

    // update view matrix
    mvp *= glm::lookAt(
        position,
        position+direction,
        up
    );
    
    // model
    mvp *= model_matrix;

    return mvp;



    // glm::mat4 mvp = model_matrix;
    
    // // right and up vectors
    // // glm::vec3 up_normal = glm::cross( direction, glm::vec3{0, 1, 0} );
    // // glm::vec3 up = glm::normalize( glm::cross( up_normal, direction ) );

    // // add view matrix
    // mvp *= glm::lookAt(
    //     position,
    //     position+direction,
    //     {0,1,0}
    // );

    // // add projection matrix
    // mvp *= glm::perspective(
    //     glm::radians(field_of_view),
    //     aspect_ratio,
    //     near_plane,
    //     far_plane
    // );

    // return mvp;
}