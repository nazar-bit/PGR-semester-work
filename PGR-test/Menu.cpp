#include "Menu.hpp"


namespace vasylnaz {

	void Menu::init(ShaderProgram& shader_manager, Camera& camera) {

		auto launch_button_mat = glm::mat4(
			4.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.0f, 1.0f, -2.0f, 1.0f);
		auto launch_button = std::make_unique<Object>("cube", launch_button_mat, "basic", "blank", "blank_norm", "blank_em");
		launch_button->scripts.push_back(std::make_unique<Launch>(launch_button.get()));
		root->addItem(std::move(launch_button), render_context);


		auto light = std::make_unique<LightSource>(
			DIRECTIONAL,
			glm::vec3(0.1f),     //amb
			glm::vec3(0.9f),     //diff
			glm::vec3(0.2f),    //spec
			glm::vec3(0.2f, 1.0f, 1.0f)   //pos
		);
		root->addItem(std::move(light), render_context);
	}
}