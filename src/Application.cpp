#include "Application.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace warp {

Application::Application()
        : _width(800),
          _height(600)
{
    // OpenGL version 4.6
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    _window = glfwCreateWindow(_width, _height, "Warp", nullptr, nullptr);
    if (!_window) {
        std::cout << "Failed to create the GLFW window\n";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        std::exit(EXIT_FAILURE);
    }

    // Handle view port dimensions
    glViewport(0, 0, _width, _height);
    glfwSetFramebufferSizeCallback(_window, [](GLFWwindow *, int width, int height) {
        glViewport(0, 0, width, height);
    });

    // center window
    const GLFWvidmode *vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int w, h;
    glfwGetWindowSize(_window, &w, &h);
    glfwSetWindowPos(_window, (vid_mode->width - w) / 2, (vid_mode->height - h) / 2);
}

Application::~Application()
{
    glfwTerminate();
}

int32_t Application::run()
{
    GLuint VAO, VBO, shader;
    GLfloat vertices[] = {
        -1.0, -1.0, 0.0,
         1.0, -1.0, 0.0,
         0.0,  1.0, 0.0,
    };
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glCreateBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::ifstream vertex("../shaders/vertex.glsl"), fragment("../shaders/fragment.glsl");
    std::stringstream vertex_shader, fragment_shader;
    vertex_shader << vertex.rdbuf();
    fragment_shader << fragment.rdbuf();

    shader = glCreateProgram();
    if (!shader)
        return -1;

    const auto add_shader = [=, this](GLuint program, const char *s, size_t s_len, GLenum type){
        GLuint _shader = glCreateShader(type);
        const GLchar *code[1];
        code[0] = s;
        GLint code_len[1];
        code_len[0] = static_cast<int>(s_len);

        glShaderSource(_shader, 1, code, (GLint *)code_len);
        glCompileShader(_shader);


        GLint res = 0;
        GLchar error_log[1024] = {};

        glGetShaderiv(_shader, GL_COMPILE_STATUS, &res);
        if (!res) {
            glGetShaderInfoLog(_shader, sizeof(error_log), nullptr, error_log);
            std::cout << error_log << std::endl;
            return -1;
        }

        glAttachShader(program, _shader);
        return 0;
    };

    add_shader(shader, vertex_shader.str().c_str(), vertex_shader.str().size(), GL_VERTEX_SHADER);
    add_shader(shader, fragment_shader.str().c_str(), fragment_shader.str().size(), GL_FRAGMENT_SHADER);

    GLint res = 0;
    GLchar error_log[1024] = {};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &res);
    if (!res) {
        glGetProgramInfoLog(shader, sizeof(error_log), nullptr, error_log);
        std::cout << error_log << std::endl;
        return -1;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &res);
    if (!res) {
        glGetProgramInfoLog(shader, sizeof(error_log), nullptr, error_log);
        std::cout << error_log << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(_window)) {
        handle_input(_window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
    return 0;
}

void Application::handle_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

} // namespace warp