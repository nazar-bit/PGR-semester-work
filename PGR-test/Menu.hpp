#pragma once

#include "SceneGraph.hpp"


namespace vasylnaz {

	class Menu : public SceneGraph {
	public:

		Menu()
			: SceneGraph()
		{
			//
		}
		 
		void init(ShaderProgram& shader_manager, Camera& camera);

	private:

		std::unique_ptr<Node> createButton(const glm::vec3& pos, const glm::vec3& scale, const std::string& text,
			const glm::vec3& pos_text, Request request, bool change_but_c);
	};
}