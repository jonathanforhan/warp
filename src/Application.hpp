#ifndef WARP_APPLICATION_HPP
#define WARP_APPLICATION_HPP

namespace warp {

class Window;
class Application final
{
public:
    explicit Application();
    ~Application();

    int run();

private:
    Window *_main_window;
};

} // namespace warp

#endif // WARP_APPLICATION_HPP
