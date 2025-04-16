#include "input.h"

std::vector<Input::Key> Input::Keys = {};

void Input::Initailize()
{
	createKeys();
}

void Input::Update(GLFWwindow* mWindow)
{
	updateKeys(mWindow);
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