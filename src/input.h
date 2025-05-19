#include "common.h"

enum class eKeyState
{
	Down,
	Pressed,
	Up,
	None,
};

enum class eMouseState
{
	Down,
	Pressed,
	Up,
	None,
};


constexpr int32_t AllKeyCodes[] = {
    GLFW_KEY_Q,
    GLFW_KEY_W,
    GLFW_KEY_E,
    GLFW_KEY_R,
    GLFW_KEY_T,
    GLFW_KEY_Y,
    GLFW_KEY_U,
    GLFW_KEY_I,
    GLFW_KEY_O,
    GLFW_KEY_P,

    GLFW_KEY_A,
    GLFW_KEY_S,
    GLFW_KEY_D,
    GLFW_KEY_F,
    GLFW_KEY_G,
    GLFW_KEY_H,
    GLFW_KEY_J,
    GLFW_KEY_K,
    GLFW_KEY_L,

    GLFW_KEY_Z,
    GLFW_KEY_X,
    GLFW_KEY_C,
    GLFW_KEY_V,
    GLFW_KEY_B,
    GLFW_KEY_N,
    GLFW_KEY_M,

    GLFW_KEY_LEFT,
    GLFW_KEY_RIGHT,
    GLFW_KEY_DOWN,
    GLFW_KEY_UP,

    GLFW_KEY_0,
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3,
    GLFW_KEY_4,
    GLFW_KEY_5,
    GLFW_KEY_6,
    GLFW_KEY_7,
    GLFW_KEY_8,
    GLFW_KEY_9,

    GLFW_KEY_SPACE,
    GLFW_KEY_ESCAPE,
    GLFW_KEY_ENTER,
    GLFW_KEY_TAB,
    GLFW_KEY_BACKSPACE,

    GLFW_KEY_LEFT_SHIFT,
    GLFW_KEY_RIGHT_SHIFT,
    GLFW_KEY_LEFT_CONTROL,
    GLFW_KEY_RIGHT_CONTROL,
    GLFW_KEY_LEFT_ALT,
    GLFW_KEY_RIGHT_ALT,

    GLFW_KEY_F1,
    GLFW_KEY_F2,
    GLFW_KEY_F3,
    GLFW_KEY_F4,
    GLFW_KEY_F5,
    GLFW_KEY_F6,
    GLFW_KEY_F7,
    GLFW_KEY_F8,
    GLFW_KEY_F9,
    GLFW_KEY_F10,
    GLFW_KEY_F11,
    GLFW_KEY_F12,
};

constexpr int32_t AllMouseButtonCodes[] = {
    GLFW_MOUSE_BUTTON_LEFT,
    GLFW_MOUSE_BUTTON_RIGHT,
    GLFW_MOUSE_BUTTON_MIDDLE,
    GLFW_MOUSE_BUTTON_4,
    GLFW_MOUSE_BUTTON_5,
};

#define class_ptr(Input)
class Input {
public:
    struct Key
    {
        int32_t keyCode;
        eKeyState state;
        bool bPressed;
    };

    struct MouseButton {
        int32_t      button;
        eMouseState  state;
        bool         bPressed;
    };

    static void Initailize();
    static void Update(GLFWwindow* mWindow);

    static bool GetKeyDown(eKeyCode code) { return Keys[static_cast<int>(code)].state == eKeyState::Down; }
    static bool GetKeyUp(eKeyCode code) { return Keys[static_cast<int>(code)].state == eKeyState::Up; }
    static bool GetKey(eKeyCode code) { return Keys[static_cast<int>(code)].state == eKeyState::Pressed; }

    // ──────────── 마우스 버튼 조회 ────────────
    static bool GetMouseButtonDown(eMouseButtonCode btn) { return MouseButtons[static_cast<int>(btn)].state == eMouseState::Down; }
    static bool GetMouseButton(eMouseButtonCode btn)     { return MouseButtons[static_cast<int>(btn)].state == eMouseState::Pressed; }
    static bool GetMouseButtonUp(eMouseButtonCode btn)   { return MouseButtons[static_cast<int>(btn)].state == eMouseState::Up; }

    // ──────────── 커서 위치 & 스크롤 오프셋 조회 ────────────
    static glm::vec2 GetMousePosition() { return mousePosition; }
    // 반환과 동시에 내부 값을 (0,0)으로 리셋
    static glm::vec2 GetScrollOffset()  { auto off = scrollOffset; scrollOffset = glm::vec2(0.0f); return off; }

    // GLFW 스크롤 콜백
    static void scrollCallback(GLFWwindow* mWindow, double xoffset, double yoffset);
    static void mousePositionCallback(GLFWwindow* mWindow, double xpos, double ypos);

private:
    static void createKeys();
    static void updateKeys(GLFWwindow* mWindow);
    static void updateKey(GLFWwindow* mWindow, Input::Key& key);
    static bool isKeyDown(GLFWwindow* mWindow, int32_t code);
    static void updateKeyDown(Input::Key& key);
    static void updateKeyUp(Input::Key& key);

    // ──────────── 마우스 내부 처리 ────────────
    static void createMouseButtons();
    static void updateMouseButtons(GLFWwindow* mWindow);
    static void updateMouseButton(GLFWwindow* mWindow, MouseButton& mb);
    static bool isMouseButtonDown(GLFWwindow* mWindow, int32_t button);
    static void updateMouseButtonDown(MouseButton& mb);
    static void updateMouseButtonUp(MouseButton& mb);

    static std::vector<Key> Keys;
    static std::vector<MouseButton> MouseButtons;
    static glm::vec2                mousePosition;
    static glm::vec2                scrollOffset;
};