#include <iostream>
#include <chrono>

#include "window.h"
#include "scene.h"

class Application {
public:
    Application(int w, int h, const char* scene_path, const char* texture_path);

    void Run();

private:
    void Draw(float time);

    Window window;
    Framebuffer<Vec4> framebuffer;
    Scene scene;
};

Application::Application(int w, int h, const char* model_path, const char* texture_path) : window("Display", w, h), framebuffer(w, h, true, 2) {
    this->scene = Scene::LoadDemoScene(model_path, texture_path);
}

void Application::Run() {
    for (float t = 0; window.Update(); t += 0.01f) {
        this->Draw(t);
    }
}

void Application::Draw(float t) {
    framebuffer.Clear();

    using namespace std::chrono;
    auto start = steady_clock::now();

    scene.Draw(framebuffer, t);

    auto end = steady_clock::now();
    auto time = duration_cast<microseconds>(end - start);

    std::cout << "\rDraw: " << time.count() / 1000.f << "ms " << std::flush;

    window.Display(framebuffer);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <model> <texture>\n";
        return 1;
    }

    Application application(1920, 1080, argv[1], argv[2]);
    application.Run();

    std::cout << '\n';
}
