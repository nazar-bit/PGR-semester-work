#pragma once

#include <vector>
#include <memory>

#include "AssetManager.hpp"
#include "Item.hpp"
#include "Object.hpp"



namespace vasylnaz {

	struct RenderContext {
		// Mesh Id lets you access objects sharing this mesh
		std::unordered_map<long, std::vector<Object*>> objects;
		LightBlock light_block;
	};



	class Node {
	public:
		/// @brief 
		/// @param model_mat 
		Node(const glm::mat4& model_mat = glm::mat4(1.0f))
			: node_id(global_node_id++), isDirty(true), model_mat(model_mat),
			global_model_mat(model_mat)
		{
			//
		}

		std::vector<std::unique_ptr<Node>> children;
		std::vector<std::unique_ptr<Item>> items;
		bool isDirty;
		const long node_id;
		glm::mat4 model_mat;
		glm::mat4 global_model_mat;

		/// @brief 
		/// @param parent_mat 
		void update(const glm::mat4& parent_mat) {
			global_model_mat = parent_mat * model_mat;
		}

		/// @brief 
		/// @param child 
		/// @return 
		Node* addChild(std::unique_ptr<Node> child);

		/// @brief 
		/// @param item 
		/// @param render_context 
		void addItem(std::unique_ptr<Item> item, RenderContext& render_context);


	private:
		static long global_node_id;
	};



	class SceneGraph {
	public:

		SceneGraph()
			: root(std::make_unique<Node>())
		{
			//
		}


		std::unique_ptr<Node> root;
		RenderContext render_context;
		

		void update() {
			update_recursive(root);
		}

		/// @brief 
		/// @param asset_manager 
		void init(AssetManager& asset_manager);


	private:
		/// @brief 
		/// @param node 
		/// @param parent_glob_mat 
		/// @param dirty 
		void update_recursive(std::unique_ptr<Node>& node, const glm::mat4& parent_glob_mat = glm::mat4(1.0f),
			bool dirty = false);
	};
}