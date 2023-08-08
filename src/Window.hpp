#pragma once
#include <functional>
#include <utility>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace warp {

class Window
{
public:
    explicit Window(GLint width = 800, GLint height = 600);
    ~Window();

    void set_current() const;
    int loop(const std::function<void()> &callback) const;
    void set_key_callback(const std::function<void(int key, int code, int action, int mode)>&);
    [[nodiscard]] const std::function<void(int, int, int, int)> &get_key_callback() const;
    [[nodiscard]] GLint get_width() const;
    [[nodiscard]] GLint get_height() const;

private:
    GLint _width;
    GLint _height;
    GLFWwindow *_window;

    bool _keys[1024];
    std::function<void(int key, int code, int action, int mode)> _key_callback;
};

} // warp
