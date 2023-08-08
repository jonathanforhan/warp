#include "Application.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Window.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

namespace warp {

Application::Application()
{
    // OpenGL version 4.6
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    _main_window = new Window();
}

Application::~Application()
{
    delete _main_window;
    glfwTerminate();
}


int32_t Application::run() {
    std::vector<GLuint> indices = {
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2
    };

    std::vector<GLfloat> vertices = {
           -1.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f
    };

    Mesh obj;
    obj.create(vertices, indices);

    Shader shader;
    shader.from_file("../shaders/vertex.glsl", "../shaders/fragment.glsl");

    _main_window->set_key_callback([&](int key, int, int action, int) {
        if (action == GLFW_PRESS) {
            std::cout << "Pressed: " << (char)key << '\n';
        } else if (action == GLFW_RELEASE) {
            std::cout << "Released: " << (char)key << '\n';
        }
    });

    return _main_window->loop([&]() {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        obj.render();
    });
}

} // namespace warp