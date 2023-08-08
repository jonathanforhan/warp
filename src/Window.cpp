#include "Window.hpp"
#include <iostream>

namespace warp {

Window::Window(GLint width, GLint height)
        : _width(width),
          _height(height),
          _window{},
          _keys{},
          _key_callback{}
{
    _window = glfwCreateWindow(width, height, "Warp", nullptr, nullptr);
    if (!_window) {
        std::cout << "Failed to create the GLFW window\n";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(_window);

    if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        std::exit(EXIT_FAILURE);
    }

    // Handle view port dimensions
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(_window, [](GLFWwindow *, int width, int height) {
        glViewport(0, 0, width, height);
    });

    // center window
    const GLFWvidmode *vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int w, h;
    glfwGetWindowSize(_window, &w, &h);
    glfwSetWindowPos(_window, (vid_mode->width - w) / 2, (vid_mode->height - h) / 2);

    glEnable(GL_DEPTH_TEST);

    glfwSetWindowUserPointer(_window, this);

    auto handle_keys = [](GLFWwindow *window, int key, int code, int action, int mode) {
        auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
        auto callback = self->get_key_callback();
        callback(key, code, action, mode);
    };
    glfwSetKeyCallback(_window, handle_keys);
}

Window::~Window()
{
    glfwDestroyWindow(_window);
}

void Window::set_current() const
{
    glfwMakeContextCurrent(_window);
}

int Window::loop(const std::function<void()> &callback) const
{
    while (!glfwWindowShouldClose(_window)) {
        glfwPollEvents();

        // main loop
        callback();

        glfwSwapBuffers(_window);
    }
    return 0;
}

void Window::set_key_callback(const std::function<void(int, int, int, int)> &callback)
{
    _key_callback = callback;
}

const std::function<void(int, int, int, int)> &Window::get_key_callback() const
{
    return _key_callback;
}

GLint Window::get_width() const
{
    return _width;
}

GLint Window::get_height() const
{
    return _height;
}

} // warp