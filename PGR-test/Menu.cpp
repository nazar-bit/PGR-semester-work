#include "Menu.hpp"


namespace vasylnaz {

	std::unique_ptr<Node> Menu::createButton(const glm::vec3& pos, const glm::vec3& scale, const std::string& text,
		const glm::vec3& pos_text, Request request, bool change_but_c) {
		auto button_node = std::make_unique<Node>();
		auto button_node_mat = glm::mat4(1.0f);
		button_node_mat = glm::translate(button_node_mat, pos);
		button_node->model_mat = button_node_mat;

		auto button_text_mat = glm::mat4(1.0f);
		button_text_mat = glm::translate(button_text_mat, pos_text);
		button_text_mat = glm::rotate(button_text_mat, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		auto button_text = createText(text, 10, MENU_TEXT_COLOR);
		button_text->model_mat = button_text_mat;
		

		auto button_mat = glm::mat4(1.0f);
		button_mat = glm::scale(button_mat, scale);
		auto button = std::make_unique<Object>("cube", button_mat, "text", "menu_button", "menu_button_norm", "blank_em");
		if (!change_but_c) {
			button->scripts.push_back(std::make_unique<RequestScript>(button.get(), request));
		}
		else {
			button->scripts.push_back(std::make_unique<RequestColorScript>(button.get(), request, button_text.get()));
		}

		button_node->addChild(std::move(button_text));
		button_node->addItem(std::move(button), render_context);
		return std::move(button_node);
	}



	void Menu::init(Camera& camera) {

		auto launch_button = createButton(glm::vec3(7.0f, 2.9f, 4.0f), glm::vec3(0.5f, 1.1f, 5.0f),
			"LAUNCH", glm::vec3(-0.3f, -0.4f, -1.5f), Request::LAUNCH, false);
		root->addChild(std::move(launch_button));

		auto debug_button = createButton(glm::vec3(7.0f, 1.4f, 4.0f-1.1f), glm::vec3(0.5f, 1.1f, 2.8f),
			"DEBUG", glm::vec3(-0.3f, -0.4f, -1.3f), Request::TOGGLE_DEBUG, true);
		root->addChild(std::move(debug_button));

		auto fog_button = createButton(glm::vec3(7.0f, 1.4f, 4.0f+1.5f), glm::vec3(0.5f, 1.1f, 2.0f),
			"FOG", glm::vec3(-0.3f, -0.4f, -0.8f), Request::TOGGLE_FOG, true);
		root->addChild(std::move(fog_button));

		auto quit_button = createButton(glm::vec3(7.0f, 0.0f, 4.0f), glm::vec3(0.5f, 1.1f, 5.0f),
			"QUIT", glm::vec3(-0.3f, -0.4f, -1.0f), Request::EXIT, false);
		root->addChild(std::move(quit_button));


		auto light = std::make_unique<LightSource>(
			DIRECTIONAL,
			glm::vec3(0.1f),     //amb
			glm::vec3(0.9f),     //diff
			glm::vec3(0.2f),    //spec
			glm::vec3(-1.0f, 2.0f, 0.0f)   //pos
		);
		
		glm::vec3 sceneCenter(7.0f, 1.45f, 4.0f);
		glm::vec3 lightPos = sceneCenter + glm::vec3(-5.0f, 10.0f, 0.0f);
		glm::vec3 upVector(0.0f, 1.0f, 0.0f);

		glm::mat4 lightView = glm::lookAt(lightPos, sceneCenter, upVector);

		float ortho_radius = 3.5f;
		float near_plane = 1.0f;
		float far_plane = 20.0f;

		glm::mat4 lightProjection = glm::ortho(-ortho_radius, ortho_radius, -ortho_radius, ortho_radius, near_plane, far_plane);
		auto lightSpaceMatrix = lightProjection * lightView;
		light->setLightSpaceMatrix(lightSpaceMatrix);

		root->addItem(std::move(light), render_context);
	}
}