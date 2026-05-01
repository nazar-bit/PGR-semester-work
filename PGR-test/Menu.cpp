#include "Menu.hpp"


namespace vasylnaz {

	void Menu::init(ShaderProgram& shader_manager, Camera& camera) {

		auto model_mat2 = glm::mat4(
			4.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.0f, 1.0f, -2.0f, 1.0f);
		auto test_object2 = std::make_unique<Object>("cube", model_mat2, "basic", "blank", "blank_norm", "blank_em");
		root->addItem(std::move(test_object2), render_context);


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