#pragma once

#include "SceneGraph.hpp"


namespace vasylnaz {

	class Menu : public SceneGraph {
	public:

		/// @brief 
		Menu()
			: SceneGraph()
		{
			//
		}
		
		/// @brief Initializes Menu's SceneGraph
		/// @param camera Camera&
		void init(Camera& camera);

	private:

		/// @brief Helper func to create Menu buttons
		/// @param pos Wold position 
		/// @param scale Button scale vec
		/// @param text Text on the button
		/// @param pos_text Relative position of text
		/// @param request What the button does
		/// @param change_but_c Change button color or not
		/// @return unique ptr to the button's Node
		std::unique_ptr<Node> createButton(const glm::vec3& pos, const glm::vec3& scale, const std::string& text,
			const glm::vec3& pos_text, Request request, bool change_but_c);
	};
}