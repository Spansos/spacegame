#pragma once

#include "graphics/camera.hpp"
#include "graphics/window.hpp"
#include "world/world.hpp"

class Player : WindowEventListener {
public:
    Player( Window & window, World & world );
    void do_input_shit();
    const Camera & get_camera();
    void resized( Window * window, glm::ivec2 size ) override;
private:
    World & _world;
    const Window & _window;
    Camera _camera;
    glm::vec3 _position;
    float _pitch, _yaw, _roll;
    float _mouse_sensitivity;
};