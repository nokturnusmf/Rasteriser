#include "window.h"

#include <GL/gl.h>
#include <GLFW/glfw3.h>

struct GLFW {
    GLFW()  { glfwInit(); }
    ~GLFW() { glfwTerminate(); }

    static void Init() { static GLFW glfw; }
};

Window* GetWindow(GLFWwindow* p) {
    return reinterpret_cast<Window*>(glfwGetWindowUserPointer(p));
}

Window::Window(const char* title, int width, int height) {
    GLFW::Init();

    this->handle = glfwCreateWindow(width, height, title, nullptr, nullptr);

    glfwMakeContextCurrent(handle);
    glfwSetWindowUserPointer(handle, this);

    this->Resize(width, height);

    glfwSetWindowSizeCallback(handle, [](auto window, int width, int height) {
        GetWindow(window)->Resize(width, height);
    });

    glfwSetCursorPosCallback(handle, [](auto window, double x, double y) {
        GetWindow(window)->SetMouse(x, y);
    });

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    this->mouse_x = 0;
    this->mouse_y = 0;

    this->width  = width;
    this->height = height;
}

Window::~Window() {
    glDeleteTextures(1, &texture);

    glfwDestroyWindow(handle);
}

void Window::Display(const Framebuffer<Vec4>& image) {
    glClear(GL_COLOR_BUFFER_BIT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.Width(), image.Height(), 0, GL_RGBA, GL_FLOAT, image.Raw());

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(-1, -1);

        glTexCoord2f(1, 0);
        glVertex2f(1, -1);

        glTexCoord2f(1, 1);
        glVertex2f(1, 1);

        glTexCoord2f(0, 1);
        glVertex2f(-1, 1);
    glEnd();

    glfwSwapBuffers(handle);
}

bool Window::Update() {
    glfwPollEvents();
    // glfwWaitEvents();

    return !glfwWindowShouldClose(handle);
}

void Window::Resize(int width, int height) {
    this->width  = width;
    this->height = height;

    glViewport(0, 0, width, height);
}

void Window::SetMouse(int mouse_x, int mouse_y) {
    this->mouse_x = mouse_x;
    this->mouse_y = mouse_y;
}

int Window::GetWidth() const {
    return this->width;
}

int Window::GetHeight() const {
    return this->height;
}

float Window::GetMouseX() const {
    return this->mouse_x;
}

float Window::GetMouseY() const {
    return this->height - this->mouse_y;
}
