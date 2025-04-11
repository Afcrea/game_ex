#include "common.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

CLASS_PTR(Ui_customize)
class Ui_customize
{
private:
    Ui_customize() {};
public:
    static bool MakeFrame();
    static bool MakeButton(const char* label, float x, float y, float s, glm::vec2 pos, std::function<void()> onClick);
    static bool EndFrame();

    static void Initialization();

    ~Ui_customize();
};