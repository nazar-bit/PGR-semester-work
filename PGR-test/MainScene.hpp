#pragma once

#include "SceneGraph.hpp"



namespace vasylnaz {

	class MainScene : public SceneGraph {
	public:

		/// @brief 
		MainScene()
			: SceneGraph()
		{
			//
		}

		/// @brief Initializes the SceneGraph
		/// @param camera Camera&
		void init(Camera& camera);

	private:
		/// @brief Helper func to initialize cubical Object
		/// @param cub Node* refering to the cubical
		/// @param script LightSwitch* script
		void addCubicalObjects(Node* cub, LightSwitch* script);

		/// @brief Helper func to initialize Windows
		/// @param render_context RenderContext& of some scene (F.E. MainScene)
		/// @param trans_vec Transposition vector
		/// @return unique ptr to the Window's node
		std::unique_ptr<Node> createWindow(RenderContext& render_context, const glm::vec3& trans_vec);

		/// @brief Helper func to initialize Fan
		/// @param root SceneGraph's root node
		void createFan(Node* root);

		/// @brief Helper func to initialize Cubicals
		/// @param root SceneGraph's root node
		void createCubicals(Node* root);
	};
}