#include <chrono>
#include <thread>
#include <iostream>

#include <glm/glm.hpp>

#include "graphics/window.hpp"
#include "graphics/renderer.hpp"
#include "world/world.hpp"
#include "player.hpp"


int main() {
    Window window{ {1024, 768}, "Spansos Space Game" };

    // Camera camera{&window};
    // camera.resized( &window, glm::ivec2{1024, 768} );
    Player player( window );

    Renderer renderer{ "resources/fragment.glsl", "resources/vertex.glsl" };

    World world;
    double last = glfwGetTime();
    // double frame_time = 1.0/120.0;

    glClearColor( .2f, .4f, .7f, .0f );
    do{
        std::cout <<  1.0/(glfwGetTime()-last) << '\n';
        last = glfwGetTime();

        glfwPollEvents( );

        player.do_input_shit();

        window.clear( .4, .5, .6 );
        world.render( window, renderer, player.get_camera() );

        window.update();
    }
    while(
        glfwGetKey(window.raw(), GLFW_KEY_ESCAPE ) != GLFW_PRESS
        &&
        !window.should_close()
    );
  
    return 0;
}