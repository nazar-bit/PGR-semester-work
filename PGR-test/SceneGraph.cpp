#include "SceneGraph.hpp"


namespace vasylnaz {

	Node* Node::addChild(std::unique_ptr<Node> child) {
		Node* ptr = child.get();
		children.push_back(std::move(child));
		isDirty = true;
		return ptr;
	}



	void Node::addItem(std::unique_ptr<Item> item, RenderContext& render_context) {
		item->updateItem(global_model_mat);
		if (Object* obj = dynamic_cast<Object*>(item.get())) {
			render_context.objects[obj->get_mesh()->mesh_id].push_back(obj);
		}
		else if (LightSource* ls = dynamic_cast<LightSource*>(item.get())) {
			render_context.light_block.addLight(ls);
		}
		items.push_back(std::move(item));
	}



	void SceneGraph::update_recursive(std::unique_ptr<Node>& node,
		const glm::mat4& parent_glob_mat, bool dirty) {

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



	void SceneGraph::init(AssetManager& asset_manager) {
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


		root->addItem(std::move(test_object), render_context);
		root->addItem(std::move(test_object2), render_context);
		auto floor_node = root->addChild(std::move(floor));
		floor_node->addItem(std::move(test_object3), render_context);


		auto testLight = std::make_unique<LightSource>(
			POINT,
			glm::vec3(0.1f),     //amb
			glm::vec3(2.0f),     //diff
			glm::vec3(1.0f),    //spec
			glm::vec3(2.0f, 2.0f, 5.0f),    //pos
			Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f),
			Attenuation(1.0f, 0.09f, 0.032f)
		);


		auto testLight2 = std::make_unique<LightSource>(
			POINT,
			glm::vec3(0.1f),     //amb
			glm::vec3(2.0f),     //diff
			glm::vec3(1.0f),    //spec
			glm::vec3(6.0f, 2.0f, 5.0f),    //pos
			Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f),
			Attenuation(1.0f, 0.09f, 0.032f)
		);


		auto testLight3 = std::make_unique<LightSource>(
			POINT,
			glm::vec3(0.1f),     //amb
			glm::vec3(2.0f),     //diff
			glm::vec3(1.0f),    //spec
			glm::vec3(6.0f, 2.0f, 8.0f),    //pos
			Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f),
			Attenuation(1.0f, 0.09f, 0.032f)
		);



		/*const LightSource testLight2(
			glm::vec3(0.1f),
			glm::vec3(0.8f),
			glm::vec3(1.0f),
			glm::vec3(0.0f, 15.0f, 0.0f),
			1.0f,
			1.0f
		);*/


		floor_node->addItem(std::move(testLight), render_context);
		floor_node->addItem(std::move(testLight2), render_context);
		floor_node->addItem(std::move(testLight3), render_context);
	}
}