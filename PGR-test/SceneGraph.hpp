#pragma once

#include <vector>
#include <memory>

#include "AssetManager.hpp"
#include "Item.hpp"
#include "Object.hpp"




namespace vasylnaz {

	class Node {
	public:

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


		void update(const glm::mat4& parent_mat) {
			global_model_mat = parent_mat * model_mat;
		}


		Node* addChild(std::unique_ptr<Node> child) {
			Node* ptr = child.get();
			children.push_back(std::move(child));
			isDirty = true;
			return ptr;
		}

		
		void addItem(std::unique_ptr<Item> item, std::unordered_map<long, std::vector<Object*>>& objects) {
			item->updateItem(global_model_mat);
			if (Object* obj = dynamic_cast<Object*>(item.get())) {
				objects[obj->get_mesh()->mesh_id].push_back(obj);
			}
			items.push_back(std::move(item));
		}


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
		// Mesh Id lets you access objects sharing this mesh
		std::unordered_map<long, std::vector<Object*>> objects;

		void update() {
			update_recursive(root);
		}



		void init(AssetManager& asset_manager) {
			// 1
			auto test_object = std::make_unique<Object>(asset_manager, "thermos", glm::mat4(1.0f), "basic", "thermos");
			// 2
			auto model_mat2 = glm::mat4(
				5.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 5.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 5.0f, 0.0f,
				5.0f, 0.0f, 0.0f, 1.0f);
			auto test_object2 = std::make_unique<Object>(asset_manager, "cube", model_mat2, "basic", "blank");
			// 3
			auto model_mat3 = glm::mat4(
				100.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.1f, 0.0f, 0.0f,
				0.0f, 0.0f, 100.0f, 0.0f,
				0.0f, -3.0f, 0.0f, 1.0f);
			auto test_object3 = std::make_unique<Object>(asset_manager, "cube", model_mat3, "red_plastic");

			// floor
			auto floor_mat = glm::mat4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 2.0f, 0.0f, 1.0f);
			auto floor = std::make_unique<Node>(floor_mat);


			root->addItem(std::move(test_object), objects);
			root->addItem(std::move(test_object2), objects);
			auto floor_node = root->addChild(std::move(floor));
			floor_node->addItem(std::move(test_object3), objects);
		}


	private:
		void update_recursive(std::unique_ptr<Node>& node, const glm::mat4& parent_glob_mat = glm::mat4(1.0f),
			bool dirty = false) {

			bool new_dirty = false;
			if (dirty || node->isDirty) {
				node->update(parent_glob_mat);
				for (std::unique_ptr<Item>& item : node->items) {
					item->updateItem(node->global_model_mat);
				}
				node->isDirty = false;
				new_dirty = true;
			}

			for (std::unique_ptr<Node>& child : node->children) {
				update_recursive(child, node->global_model_mat, new_dirty);
			}
		}
	};
}