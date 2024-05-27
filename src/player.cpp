#include "player.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
#include <cmath>

Player::Player( Window & window, World & world ) :
    _world(world),
    _window(window)
{
    window.subscribe( this );
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

    _camera.direction = glm::rotateY( glm::rotateZ( glm::vec3{1,0,0}, glm::radians(_pitch) ), glm::radians(_yaw) );
    _camera.roll = _roll;

    glm::vec3 forward = glm::normalize(_camera.direction);
    glm::vec3 right = glm::normalize(glm::cross(_camera.direction, glm::vec3{0,1,0}));

    if ( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
        _position += forward;
    if ( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
        _position += -right;
    if ( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
        _position += -forward;
    if ( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
        _position += right;
    if ( glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS )
        _roll++;
    if ( glfwGetKey( window, GLFW_KEY_Q ) == GLFW_PRESS )
        _roll--;

    if ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) ) {
        auto r = _world.raycast( _position, forward );
        if ( r.has_value() ) {
            glm::ivec3 int_pos = { std::round<int>(r.value().x),std::round<int>(r.value().y),std::round<int>(r.value().z) };
            for ( int x=-11; x<=11; x++ ) {
                for ( int y=-11; y<=11; y++ ) {
                    for ( int z=-11; z<=11; z++ ) {
                        auto a = _world.calc_chunk_coordinates(int_pos+glm::ivec3{x,y,z});
                        _world.get_chunk(a.first).get_voxel(a.second) += 0.05;///glm::length(glm::vec3{x,y,z});
                    }
                }
            }
            for ( int x=-2; x<=2; x++ ) {
                for ( int y=-2; y<=2; y++ ) {
                    for ( int z=-2; z<=2; z++ ) {
                        auto a = _world.calc_chunk_coordinates(int_pos);
                        _world.get_chunk(a.first+glm::ivec3{x,y,z}).cube_march();
                    }
                }
            }
        }
    }

    std::cout << "player: " << _pitch << ' ' << _yaw << std::endl;
    std::cout << "camera: " << _camera.direction.x << ' ' << _camera.direction.y << ' ' << _camera.direction.z << std::endl;

    _camera.position = _position;
}


const Camera & Player::get_camera() {
    return _camera;
}

void Player::resized( Window * window, glm::ivec2 size ) {
    (void)window;
    _camera.aspect_ratio = (float)size.x/size.y;
}