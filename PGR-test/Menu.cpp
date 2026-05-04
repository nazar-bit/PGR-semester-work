#include "Menu.hpp"


namespace vasylnaz {

	std::unique_ptr<Node> Menu::createButton(const glm::vec3& pos, const std::string& text,
		const glm::vec3& pos_text, Request request) {
		auto button_node = std::make_unique<Node>();
		auto button_node_mat = glm::mat4(1.0f);
		button_node_mat = glm::translate(button_node_mat, pos);
		button_node->model_mat = button_node_mat;

		auto button_mat = glm::mat4(1.0f);
		button_mat = glm::scale(button_mat, glm::vec3(0.5f, 1.1f, 5.0f));
		auto button = std::make_unique<Object>("cube", button_mat, "text", "menu_button", "menu_button_norm", "blank_em");
		button->scripts.push_back(std::make_unique<RequestScript>(button.get(), request));
		button_node->addItem(std::move(button), render_context);

		auto button_text_mat = glm::mat4(1.0f);
		button_text_mat = glm::translate(button_text_mat, pos_text);
		button_text_mat = glm::rotate(button_text_mat, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		auto button_text = createText(text, 10, glm::vec3(0.5f, 0.0f, 0.0f));
		button_text->model_mat = button_text_mat;
		button_node->addChild(std::move(button_text));

		return std::move(button_node);
	}



	void Menu::init(ShaderProgram& shader_manager, Camera& camera) {

		auto launch_button = createButton(glm::vec3(7.0f, 2.9f, 4.0f), "LAUNCH",
			glm::vec3(-0.3f, -0.4f, -1.5f), Request::LAUNCH);
		root->addChild(std::move(launch_button));

		auto settings_button = createButton(glm::vec3(7.0f, 1.4f, 4.0f), "DEBUG",
			glm::vec3(-0.3f, -0.4f, -1.4f), Request::ACTIVATE_DEBUG);
		root->addChild(std::move(settings_button));

		auto quit_button = createButton(glm::vec3(7.0f, 0.0f, 4.0f), "QUIT",
			glm::vec3(-0.3f, -0.4f, -1.0f), Request::EXIT);
		root->addChild(std::move(quit_button));


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