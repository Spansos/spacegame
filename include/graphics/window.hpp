#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Window;

class WindowEventListener {
public:
    virtual void resized( Window * window, glm::ivec2 size ) = 0;
};

class Window {
public:
    Window( glm::ivec2 resolution, std::string title, int antialiasing=4, int gl_version_major=4, int gl_version_minor=0 );
    ~Window();
    void clear( float red, float green, float blue );
    void update();
    void subscribe( WindowEventListener * listener );
    bool should_close();
    GLFWwindow * raw() {
        return _raw_window;
    }
private:
    static void framebuffer_resize_callback( GLFWwindow * window, int width, int height );
    static std::vector<Window*> _windows;
    std::vector<WindowEventListener*> _listeners;
    GLFWwindow * _raw_window;
};