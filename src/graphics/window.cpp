#include "graphics/window.hpp"
#include <stdexcept>

std::vector<Window*> Window::_windows = {};

Window::Window( glm::ivec2 resolution, std::string title, int antialiasing, int gl_version_major, int gl_version_minor ) {
    glewExperimental = true;
    const char * error;

    // glfw init
    glfwInit();
    if( glfwGetError( &error ) ) {
        glfwTerminate();
        throw std::runtime_error( error );
    }

    // window creation
    glfwWindowHint( GLFW_SAMPLES, antialiasing ); // antialiasing
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, gl_version_major ); // opengl version
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, gl_version_minor );
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // We want core functionality (instead of older something-something (which is higher level))
    _raw_window = glfwCreateWindow( resolution.x, resolution.y, title.c_str(), nullptr, nullptr );
    if( glfwGetError( &error ) ) {
        glfwTerminate();
        throw std::runtime_error( std::string(error) );
    }

    // glew init
    glfwMakeContextCurrent( _raw_window );
    GLenum err = glewInit();
    if ( err != GLEW_OK ) {
        glfwTerminate();
        throw std::runtime_error( error );
    }
    
    glfwSetInputMode(_raw_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetFramebufferSizeCallback( _raw_window, Window::framebuffer_resize_callback );

    _windows.push_back( this );
}

Window::~Window() {
    glfwDestroyWindow( _raw_window );
    glfwTerminate( );
}

void Window::clear( float red, float green, float blue ) {
    glClearColor( red, green, blue, 0 );
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
}

void Window::update() {
    glfwSwapBuffers(_raw_window);
}

void Window::subscribe( WindowEventListener * listener ) {
    _listeners.push_back( listener );
}

bool Window::should_close() {
    return glfwWindowShouldClose( _raw_window );
}

void Window::framebuffer_resize_callback( GLFWwindow * raw_window, int width, int height ) {
    glViewport( 0, 0, width, height );
    for ( Window * window : _windows ) {
        if ( window->_raw_window == raw_window ) {
            for ( WindowEventListener * listener : window->_listeners ) {
                listener->resized( window, { width, height } );
            }
            break;
        }
    }
}