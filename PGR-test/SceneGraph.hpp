#pragma once

#include <vector>
#include <memory>
#include <string>
#include <glm/gtx/rotate_vector.hpp>

#include "AssetManager.hpp"
#include "Item.hpp"
#include "Object.hpp"
#include "Light.hpp"
#include "NodeScript.hpp"



namespace vasylnaz {

	class Node;


	struct RenderContext {
		std::unordered_map<RenderQueue, std::vector<Object*>> objects;
		LightBlock light_block;
	};


	enum Actions {
		CLICK,
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
		std::vector<std::unique_ptr<NodeScript>> scripts;

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
		/// @param render 
		void addItem(std::unique_ptr<Item> item, RenderContext& render_context, bool render = true);

		/// @brief 
		void renderItems(RenderContext& render_context);


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

		/// @brief loads an OBJ file by reading all meshes, textures, creating objects.
		/// @param filepath 
		/// @param asset_manager 
		/// @return Node* items of which consist of objects created from .obj.
		std::unique_ptr<Node> loadOBJ(const std::string& filepath,
			const unsigned int from, const unsigned int to, bool render = true);

		/// @brief 
		/// @param asset_manager 
		void init(ShaderProgram& shader_manager);

		/// @brief 
		/// @param id 
		/// @param action 
		void findObject(const long id, const Actions action);

		
		


	private:
		/// @brief 
		/// @param node 
		/// @param parent_glob_mat 
		/// @param dirty 
		void update_recursive(std::unique_ptr<Node>& node, const glm::mat4& parent_glob_mat = glm::mat4(1.0f),
			bool dirty = false);

		/// @brief 
		/// @param id 
		/// @param action 
		/// @param node 
		bool findObjectRecursive(long id, Actions action, std::unique_ptr<Node>& node);

		void addCubicalObjects(Node* cub, LightSwitch* script);

		std::unique_ptr<Node> createWindow(RenderContext& render_context, const glm::vec3& trans_vec);

		void createFan(Node* root);

		void createCubicals(Node* root);
	};
}