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
		else if (button == GLUT_LEFT_BUTTON) {
			mouse.last_coords = glm::vec2(x, y);
			mouse.new_coords = glm::vec2(x, y);
			if (state == GLUT_DOWN) {
				mouse.buttons_state[button] = true;
			}
		}
	}


	void InputHandler::pick(const PickingProgram& pick_prog, const SceneGraph& scene_graph,
		const glm::mat4& view_mat, const glm::mat4& proj_mat) {
		if (!mouse.buttons_state[GLUT_LEFT_BUTTON]) {
			return;
		}


		glDisable(GL_BLEND);
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glActiveTexture(GL_TEXTURE0); 
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, 1);
		glBindTexture(GL_TEXTURE_2D, 2);


		mouse.buttons_state[GLUT_LEFT_BUTTON] = false;
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(pick_prog.shaderProgram);
		glUniformMatrix4fv(pick_prog.positionP, 1, GL_FALSE, glm::value_ptr(proj_mat));

		for (const auto& obj : scene_graph.render_context.objects.at(RenderQueue::OPAQUE_MASK)) {
			obj->pickRender(pick_prog, view_mat);
		}

		for (const auto& obj : scene_graph.render_context.objects.at(RenderQueue::TRANSPARENT_MASK)) {
			obj->pickRender(pick_prog, view_mat);
		}
		
		glFinish();
		unsigned char pixel[4];
		glReadBuffer(GL_BACK);
		glReadPixels(mouse.last_coords.x, WIN_HEIGHT - 1 - mouse.last_coords.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

		if (pixel[1] == 0) {
			std::cout << "clicked on background" << std::endl;
		}
		else {
			std::cout << "clicked on object " << (int)pixel[0]
				<< " in depth " << (float)pixel[2] * MAX_DEPTH / 255
				<< std::endl;
		}

		glutPostRedisplay();
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