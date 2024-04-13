#include "Input.h"
#include "Helper.h"

void Input::Init()
{
	//set all keycode status as up
	for (int code = SDL_SCANCODE_UNKNOWN; code < SDL_NUM_SCANCODES; code++) {
		keyboard_states[static_cast<SDL_Scancode>(code)] = INPUT_STATE_UP;
	}
	for (int i = 1; i <= 3; i++) {
		mouse_button_states[i] = INPUT_STATE_UP;
	}
}

void Input::ProcessEvent(const SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		keyboard_states[e.key.keysym.scancode] = INPUT_STATE_JUST_BECAME_DOWN;
		just_became_down_scancodes.push_back(e.key.keysym.scancode);
		break;
	case SDL_KEYUP:
		keyboard_states[e.key.keysym.scancode] = INPUT_STATE_JUST_BECAME_UP;
		just_became_up_scancodes.push_back(e.key.keysym.scancode);
		break;
	case SDL_MOUSEMOTION:
		mouse_position.x = e.motion.x;
		mouse_position.y = e.motion.y;
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (e.button.button == SDL_BUTTON_LEFT) {
			mouse_button_states[e.button.button] = INPUT_STATE_JUST_BECAME_DOWN;
			just_became_down_buttons.push_back(e.button.button);
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			mouse_button_states[e.button.button] = INPUT_STATE_JUST_BECAME_DOWN;
			just_became_down_buttons.push_back(e.button.button);
		}
		if (e.button.button == SDL_BUTTON_MIDDLE) {
			mouse_button_states[e.button.button] = INPUT_STATE_JUST_BECAME_DOWN;
			just_became_down_buttons.push_back(e.button.button);
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (e.button.button == SDL_BUTTON_LEFT) {
			mouse_button_states[e.button.button] = INPUT_STATE_JUST_BECAME_UP;
			just_became_up_buttons.push_back(e.button.button);
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			mouse_button_states[e.button.button] = INPUT_STATE_JUST_BECAME_UP;
			just_became_up_buttons.push_back(e.button.button);
		}
		if (e.button.button == SDL_BUTTON_MIDDLE) {
			mouse_button_states[e.button.button] = INPUT_STATE_JUST_BECAME_UP;
			just_became_up_buttons.push_back(e.button.button);
		}
		break;
	case SDL_MOUSEWHEEL:
		mouse_scroll_this_frame = e.wheel.preciseY; 
		break;
	}


}

void Input::LateUpdate()
{
	for (const SDL_Scancode& code : just_became_down_scancodes) {
		keyboard_states[code] = INPUT_STATE_DOWN;
	}
	just_became_down_scancodes.clear();

	for (const SDL_Scancode& code : just_became_up_scancodes) {
		keyboard_states[code] = INPUT_STATE_UP;
	}
	just_became_up_scancodes.clear();

	for (const int& button : just_became_down_buttons) {
		mouse_button_states[button] = INPUT_STATE_DOWN;
	}
	just_became_down_buttons.clear();

	for (const int& button : just_became_up_buttons) {
		mouse_button_states[button] = INPUT_STATE_UP;
	}
	just_became_up_buttons.clear();
}

bool Input::GetKey(const std::string& keycode)
{
	if (__keycode_to_scancode.find(keycode) == __keycode_to_scancode.end()) return false;
	SDL_Scancode scancode = __keycode_to_scancode.at(keycode);
	return keyboard_states[scancode] == INPUT_STATE_DOWN || keyboard_states[scancode] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetKeyDown(const std::string& keycode)
{
	if (__keycode_to_scancode.find(keycode) == __keycode_to_scancode.end()) return false;
	SDL_Scancode scancode = __keycode_to_scancode.at(keycode);
	return keyboard_states[scancode] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetKeyUp(const std::string& keycode)
{
	if (__keycode_to_scancode.find(keycode) == __keycode_to_scancode.end()) return false;
	SDL_Scancode scancode = __keycode_to_scancode.at(keycode);
	return keyboard_states[scancode] == INPUT_STATE_JUST_BECAME_UP;
}

glm::vec2 Input::GetMousePosition()
{
	return mouse_position;
}

bool Input::GetMouseButton(int button)
{
	return mouse_button_states[button] == INPUT_STATE_DOWN || mouse_button_states[button] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetMouseButtonDown(int button)
{
	return mouse_button_states[button] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetMouseButtonUp(int button)
{
	return mouse_button_states[button] == INPUT_STATE_JUST_BECAME_UP;
}

void Input::ResetMouseScrollDelta()
{
	mouse_scroll_this_frame = 0.0f;
}

float Input::GetMouseScrollDelta()
{
	return mouse_scroll_this_frame;
}
