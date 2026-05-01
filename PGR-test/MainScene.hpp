#pragma once

#include "SceneGraph.hpp"



namespace vasylnaz {

	class MainScene : public SceneGraph {
	public:

		MainScene()
			: SceneGraph()
		{
			//
		}

		/// @brief 
		/// @param asset_manager 
		void init(ShaderProgram& shader_manager, Camera& camera);

	private:
		void addCubicalObjects(Node* cub, LightSwitch* script);

		std::unique_ptr<Node> createWindow(RenderContext& render_context, const glm::vec3& trans_vec);

		void createFan(Node* root);

		void createCubicals(Node* root);
	};
}