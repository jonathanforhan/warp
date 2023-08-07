#ifndef WARP_APPLICATION_HPP
#define WARP_APPLICATION_HPP

#include <cstdint>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace warp {

class Application final
{
public:
    explicit Application();
    ~Application();

    int32_t run();
    static void handle_input(GLFWwindow *window);

private:
    int32_t _width, _height;
    GLFWwindow *_window;
};

} // namespace warp

#else
#error("Cannot define application twice")
#endif // WARP_APPLICATION_HPP
