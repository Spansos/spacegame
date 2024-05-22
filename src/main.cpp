#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <unordered_set>

#include "graphics/window.hpp"
#include "graphics/camera.hpp"
#include "graphics/renderer.hpp"
#include "graphics/mesh.hpp"
#include "world/world.hpp"

#include <iostream>

int main() {
    Window window{ {1024, 768}, "Spansos Space Game" };

    Camera camera{&window};
    camera.resized( &window, glm::ivec2{1024, 768} );

    Renderer renderer{ "resources/fragment.glsl", "resources/vertex.glsl" };

    World world;

    glClearColor( .2f, .4f, .7f, .0f );
    do{
        glfwPollEvents( );

        camera.do_input_shit();
        window.clear( .4, .5, .6 );
        world.render( window, renderer, camera );

        window.update();
    }
    while(
        glfwGetKey(window.raw(), GLFW_KEY_ESCAPE ) != GLFW_PRESS
        &&
        !window.should_close()
    );
  
    return 0;
}