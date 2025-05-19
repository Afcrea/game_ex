#include "input.h"

std::vector<Input::Key> Input::Keys = {};
std::vector<Input::MouseButton> Input::MouseButtons;
glm::vec2                       Input::mousePosition = glm::vec2(0.0f);
glm::vec2                       Input::scrollOffset  = glm::vec2(0.0f);

void Input::Initailize()
{
	createKeys();
	createMouseButtons();
}

void Input::Update(GLFWwindow* mWindow)
{
	updateKeys(mWindow);
	updateMouseButtons(mWindow);
}
void Input::createKeys()
{
	for (auto keyCode : AllKeyCodes)
	{
		Key key = {};
		key.bPressed = false;
		key.state = eKeyState::None;
		key.keyCode = static_cast<int32_t>(keyCode);

		Keys.push_back(key);
	}
}

void Input::updateKeys(GLFWwindow* mWindow)
{
	std::for_each(Keys.begin(), Keys.end(), 
		[&mWindow](Key& key) -> void
		{
			updateKey(mWindow, key);
		});
}

void Input::updateKey(GLFWwindow* mWindow, Input::Key& key)
{
	if (isKeyDown(mWindow, key.keyCode))
	{
		updateKeyDown(key);
	}
	else 
	{
		updateKeyUp(key);
	}
}

bool Input::isKeyDown(GLFWwindow* mWindow, int32_t code)
{
	return glfwGetKey(mWindow, static_cast<int>(code)) == GLFW_PRESS;
}

void Input::updateKeyDown(Input::Key& key)
{
	if (key.bPressed == true)
		key.state = eKeyState::Pressed;
	else
		key.state = eKeyState::Down;

	key.bPressed = true;
}
void Input::updateKeyUp(Input::Key& key)
{
	if (key.bPressed == true)
		key.state = eKeyState::Up;
	else
		key.state = eKeyState::None;

	key.bPressed = false;
}

// ──────────── 마우스 내부 구현 ────────────
void Input::createMouseButtons()
{
    for (int btn : AllMouseButtonCodes)
    {
        MouseButton mb{};
        mb.button   = btn;
        mb.bPressed = false;
        mb.state    = eMouseState::None;
        MouseButtons.push_back(mb);
    }
}

void Input::updateMouseButtons(GLFWwindow* mWindow)
{
    for (auto& mb : MouseButtons)
        updateMouseButton(mWindow, mb);
}

void Input::updateMouseButton(GLFWwindow* mWindow, Input::MouseButton& mb)
{
    if (isMouseButtonDown(mWindow, mb.button))
        updateMouseButtonDown(mb);
    else
        updateMouseButtonUp(mb);
}

bool Input::isMouseButtonDown(GLFWwindow* mWindow, int32_t button)
{
    return glfwGetMouseButton(mWindow, button) == GLFW_PRESS;
}

void Input::updateMouseButtonDown(Input::MouseButton& mb)
{
    mb.state    = mb.bPressed ? eMouseState::Pressed : eMouseState::Down;
    mb.bPressed = true;
}

void Input::updateMouseButtonUp(Input::MouseButton& mb)
{
    mb.state    = mb.bPressed ? eMouseState::Up : eMouseState::None;
    mb.bPressed = false;
}

void Input::scrollCallback(GLFWwindow* /*mWindow*/, double xoffset, double yoffset)
{
    scrollOffset.x = static_cast<float>(xoffset);
    scrollOffset.y = static_cast<float>(yoffset);
}

void Input::mousePositionCallback(GLFWwindow* /*mWindow*/, double xpos, double ypos)
{
	mousePosition.x = static_cast<float>(xpos);
	mousePosition.y = static_cast<float>(ypos);
}