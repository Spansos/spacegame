#include "graphics/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Camera::Camera( Window * window ) : _window(window) {
    window->subscribe( this );
}

void Camera::do_input_shit() {
    auto *window = glfwGetCurrentContext( );

    // get mouse movement
    double xpos, ypos;
    glfwGetCursorPos( window, &xpos, &ypos );
    glm::vec2 delta_mouse = glm::vec2{ xpos, ypos } - _prev_mouse_pos;
    _prev_mouse_pos = { xpos, ypos };

    // right and up vectors
    glm::vec3 dpoi = _position - _point_of_intereset;
    glm::vec3 right = glm::normalize(glm::cross( dpoi, glm::vec3{0, 1, 0} ));
    glm::vec3 up = glm::normalize(glm::cross( right, dpoi ));

    // orbit view
    if ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS ) {
        float length = glm::length( dpoi );
        _position += (right*delta_mouse.x + up*delta_mouse.y) * _orbit_speed;
        _position = _point_of_intereset + glm::normalize( _position-_point_of_intereset )*length;
    }
    // zoom
    if ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_MIDDLE ) == GLFW_PRESS ) {
        glm::vec3 forward = glm::normalize( glm::cross( up, right ) );
        _position += forward * delta_mouse.y * _zoom_speed;
    }
    // pan poi
    if ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS ) {
        _point_of_intereset += (right*delta_mouse.x + up*delta_mouse.y) * _pan_speed;
        _position += (right*delta_mouse.x + up*delta_mouse.y) * _pan_speed;
    }

    // update view matrix
    _view = glm::lookAt(
        _position,
        _point_of_intereset,
        up
    );
}

glm::mat4 Camera::calcMvp( glm::mat4 model_matrix ) const {
    return _projection * _view * model_matrix;
}

void Camera::resized( Window * window, glm::ivec2 size ) {
    (void)window;
    _projection = glm::perspective(
        glm::radians(_FoV),
        (float)size.x/size.y,
        0.1f,
        250.0f
    );
}