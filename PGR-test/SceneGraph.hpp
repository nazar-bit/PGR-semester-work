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
#include "Curve.hpp"
#include "Params.hpp"
#include "Camera.hpp"


#define CHAR_W  5
#define CHAR_H  7
#define CHAR_PADDING 1
#define FONT_W 128
#define FONT_H 64
#define FONT_SCALE_FACTOR 0.01
#define TOTAL_ROWS 7
#define FIRST_CHAR_ID 32
#define N_CHAR_IN_ROW 18



namespace vasylnaz {

	class Node;


	struct RenderContext {
		std::unordered_map<RenderQueue, std::vector<Object*>> objects;
		std::vector<Curve*> curves;
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
			updateRecursive(root);
		}

		/// @brief loads an OBJ file by reading all meshes, textures, creating objects.
		/// @param filepath 
		/// @param asset_manager 
		/// @return Node* items of which consist of objects created from .obj.
		std::unique_ptr<Node> loadOBJ(const std::string& filepath,
			const unsigned int from = 0, const unsigned int to = 10000, bool render = true);

		/// @brief creates a Text Node from string
		/// @param text 
		/// @return Node with text quads
		std::unique_ptr<Node> createText(const std::string& text, const unsigned int font_size,
			const glm::vec3& font_color);

		/// @brief 
		/// @param id 
		/// @param action 
		void findObject(const long id, const Actions action);


		/// @brief 
		/// @param asset_manager 
		virtual void init(ShaderProgram& shader_manager, Camera& camera) = 0;
		


	private:
		/// @brief 
		/// @param node 
		/// @param parent_glob_mat 
		/// @param dirty 
		void updateRecursive(std::unique_ptr<Node>& node, const glm::mat4& parent_glob_mat = glm::mat4(1.0f),
			bool dirty = false);

		/// @brief 
		/// @param id 
		/// @param action 
		/// @param node 
		bool findObjectRecursive(long id, Actions action, std::unique_ptr<Node>& node);
	};
}