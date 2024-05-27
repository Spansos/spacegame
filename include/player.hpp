#pragma once

#include "graphics/camera.hpp"
#include "graphics/window.hpp"

class Player {
public:
    Player( const Window & window );
    void do_input_shit();
    const Camera & get_camera();
private:
    const Window & _window;
    Camera _camera;
    glm::vec3 _position;
    float _pitch, _yaw, _roll;
    float _mouse_sensitivity;
};