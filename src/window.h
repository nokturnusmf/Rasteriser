#pragma once

#include "framebuffer.h"
#include "matrix.h"

struct GLFWwindow;

class Window {
public:
    Window(const char* title, int width, int height);
    ~Window();

    Window(const Window&) = delete;
    Window(Window&&) = delete;

    bool Update();

    void Display(const Framebuffer<Vec4>& image);

    int GetWidth() const;
    int GetHeight() const;

    float GetMouseX() const;
    float GetMouseY() const;

private:
    void Resize(int width, int height);
    void SetMouse(int mouse_x, int mouse_y);

    GLFWwindow* handle;
    unsigned int texture;

    int width;
    int height;

    int mouse_x;
    int mouse_y;
};
