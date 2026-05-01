#include "Menu.hpp"


namespace vasylnaz {

	void Menu::init(ShaderProgram& shader_manager, Camera& camera) {

		auto launch_button_mat = glm::mat4(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 4.0f, 0.0f,
			5.0f, 2.0f, 4.0f, 1.0f);
		auto launch_button = std::make_unique<Object>("cube", launch_button_mat, "basic", "blank", "blank_norm", "blank_em");
		launch_button->scripts.push_back(std::make_unique<Launch>(launch_button.get()));
		root->addItem(std::move(launch_button), render_context);


		auto launch_button_text_mat = glm::mat4(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 4.0f, 0.0f,
			4.8f, 2.0f, 4.0f, 1.0f);
		auto launch_button_text = createText("abcdefr\nsdssd\n", 50, glm::vec3(1.0f));
		launch_button_text->model_mat = launch_button_text_mat;
		root->addChild(std::move(launch_button_text));


		auto light = std::make_unique<LightSource>(
			DIRECTIONAL,
			glm::vec3(0.1f),     //amb
			glm::vec3(0.9f),     //diff
			glm::vec3(0.2f),    //spec
			glm::vec3(-1.0f, 2.0f, 0.0f)   //pos
		);
		root->addItem(std::move(light), render_context);
	}
}