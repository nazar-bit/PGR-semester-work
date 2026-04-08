#include "InputHandler.hpp"


namespace vasylnaz {

	InputHandler::InputHandler()
	{
		keys_state = { false };
		mouse.buttons_state = { false };
		mouse.last_coords = glm::vec2(0, 0);
		mouse.new_coords = glm::vec2(0, 0);
	}


	void InputHandler::handle_mouse_click(const int& button, const int& state, const int& x, const int& y) {
		if (button == GLUT_RIGHT_BUTTON) {
			mouse.last_coords = glm::vec2(x, y);
			mouse.new_coords = glm::vec2(x, y);
			if (state == GLUT_DOWN) {
				mouse.buttons_state[button] = true;
			}
			else if (state == GLUT_UP) {
				mouse.buttons_state[button] = false;
			}
		}
	}


	void InputHandler::handle_mouse_drag(const int& x, const int& y) {
		if (mouse.buttons_state[GLUT_RIGHT_BUTTON]) {
			mouse.new_coords = glm::vec2(x, y);
		}
	}


	void InputHandler::update_camera_pos(Camera& camera) {
		glm::vec3 forward = -camera.target;
		glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));

		if (keys_state['w'] || special_keys_state[GLUT_KEY_UP])
		{
			camera.position += forward * camera.movement_speed;
		}
		if (keys_state['s'] || special_keys_state[GLUT_KEY_DOWN])
		{
			camera.position -= forward * camera.movement_speed;
		}
		if (keys_state['a'] || special_keys_state[GLUT_KEY_LEFT])
		{
			camera.position -= right * camera.movement_speed;
		}
		if (keys_state['d'] || special_keys_state[GLUT_KEY_RIGHT])
		{
			camera.position += right * camera.movement_speed;
		}
	}


	void InputHandler::update_camera_target(Camera& camera) {
		if (mouse.last_coords == mouse.new_coords) {
			return;
		}
		glm::vec3 forward = -camera.target;
		glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));

		float rotation_angle = glm::distance(mouse.last_coords, mouse.new_coords) * camera.rotation_angle;
		glm::vec2 direction_coef = glm::normalize(mouse.last_coords - mouse.new_coords);
		glm::vec3 rotation_dir = camera.up * direction_coef.x + right * direction_coef.y;

		camera.target = glm::rotate(camera.target, glm::radians(rotation_angle), rotation_dir);
		right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
		camera.up = glm::cross(-forward, right);

		camera.target = glm::normalize(camera.target);
		camera.up = glm::normalize(camera.up);

		mouse.last_coords = mouse.new_coords;
	}
}