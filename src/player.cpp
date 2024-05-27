#include "player.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
#include <cmath>

Player::Player( const Window & window ) :
    _window(window)
{
    _mouse_sensitivity = 0.5;
    _position = {0, 0, 100};
    _pitch = 0;
    _yaw = 0;
    _roll = 0;
    _camera = { .position={0,0,100}, .direction={0,0,-1}, .roll=0, .field_of_view=45.0, .aspect_ratio=1.33, .near_plane=.1, .far_plane=250.0 };
}

void Player::do_input_shit() {
    auto *window = glfwGetCurrentContext( );
    glm::ivec2 res;
    glfwGetWindowSize( window, &res.x, &res.y );
    glm::vec2 mid = res/2;

    // get mouse movement
    double xpos, ypos;
    glfwGetCursorPos( window, &xpos, &ypos );
    glm::vec2 delta_mouse = glm::vec2{ xpos, ypos } - mid;
    // set mouse to mid of window
    glfwSetCursorPos( window, mid.x, mid.y );

    // rotate camera
    _pitch -= delta_mouse.y * _mouse_sensitivity;
    _yaw -= delta_mouse.x * _mouse_sensitivity;

    _pitch = std::fmod(std::fmod(_pitch,360)+360,360);;
    _yaw = std::fmod(std::fmod(_yaw,360)+360,360);

    _camera.direction = glm::rotateX( glm::vec3{1,0,0}, _pitch )
    _camera.roll = _roll;

    if ( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS ) {
        _position += _camera.direction;
    }

    std::cout << "player: " << _pitch << ' ' << _yaw << std::endl;
    std::cout << "camera: " << _camera.direction.x << ' ' << _camera.direction.y << ' ' << _camera.direction.z << std::endl;

    _camera.position = _position;
}


const Camera & Player::get_camera() {
    return _camera;
}