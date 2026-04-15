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



	std::unique_ptr<Node> SceneGraph::loadOBJ(const std::string& filepath, ShaderManager& shader_manager) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filepath,
			aiProcess_Triangulate |           // Transform all primitives to triangles
			aiProcess_JoinIdenticalVertices | // Merge identical vertices
			aiProcess_CalcTangentSpace        // Calculate tangents and bitangents
		);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "ASSIMP Error: " << importer.GetErrorString() << std::endl;
			return nullptr;
		}
		std::cout << "Successfully loaded: " << filepath << std::endl;
		std::cout << "Total meshes found: " << scene->mNumMeshes << std::endl;


		auto node = std::make_unique<Node>();


		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			aiMesh* mesh = scene->mMeshes[i];

			auto mesh_name = mesh->mName.C_Str();
			auto msh = std::make_unique<Mesh>(mesh, shader_manager);
			AssetManager::getInstance().loadMesh(mesh_name, std::move(msh));

			unsigned int materialIndex = mesh->mMaterialIndex;

			if (materialIndex >= 0 && materialIndex < scene->mNumMaterials) {
				aiMaterial* material = scene->mMaterials[materialIndex];
				aiString texturePath;
				aiString materialName;
				bool hasDifTex = true;
				// Get texture name
				if (material->Get(AI_MATKEY_NAME, materialName) == AI_SUCCESS) {
					std::cout << "Object " << mesh_name
						<< " uses Texture: " << materialName.C_Str() << std::endl;
				}
				else {
					std::cout << "Object " << mesh_name << " uses an unnamed material." << std::endl;
					hasDifTex = false;
				}

				// Get texture
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {

					std::cout << "Object " << mesh_name
						<< " uses Diffuse Texture: " << texturePath.C_Str() << std::endl;
					AssetManager::getInstance().loadTetxure(materialName.C_Str(), texturePath.C_Str());
				}
				else {
					std::cout << "Object " << mesh_name << " has no diffuse texture." << std::endl;
					hasDifTex = false;
				}

				std::unique_ptr<Object> object;
				if (hasDifTex) {
					object = std::make_unique<Object>(mesh_name, glm::mat4(1.0f),
						"basic", materialName.C_Str());
				}
				else {
					object = std::make_unique<Object>(mesh_name, glm::mat4(1.0f));
				}

				node->addItem(std::move(object), render_context);
			}
		}

		return node;
	}



	void SceneGraph::init(ShaderManager& shader_manager) {
		// 1
		auto test_object = std::make_unique<Object>("thermos", glm::mat4(1.0f), "basic", "thermos");
		// 2
		auto model_mat2 = glm::mat4(
			5.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 5.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 5.0f, 0.0f,
			5.0f, 0.0f, 0.0f, 1.0f);
		auto test_object2 = std::make_unique<Object>("cube", model_mat2, "basic", "blank");
		// 3
		auto model_mat3 = glm::mat4(
			100.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.1f, 0.0f, 0.0f,
			0.0f, 0.0f, 100.0f, 0.0f,
			0.0f, -3.0f, 0.0f, 1.0f);
		auto test_object3 = std::make_unique<Object>("cube", model_mat3, "red_plastic");

		// floor
		auto floor_mat = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 2.0f, 0.0f, 1.0f);
		auto floor = std::make_unique<Node>(floor_mat);

		//ground_obj
		auto ground_obj = std::make_unique<Object>("ground", glm::mat4(1.0f), "basic", "rocks", "rocks_norm");
		root->addItem(std::move(ground_obj), render_context);

		root->addItem(std::move(test_object), render_context);
		root->addItem(std::move(test_object2), render_context);
		auto floor_node = root->addChild(std::move(floor));
		floor_node->addItem(std::move(test_object3), render_context);

		auto grass = loadOBJ("Models/searsia_lucida_1k.obj", shader_manager);
		auto grass_mat = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, -0.8f, 0.0f, 1.0f);
		grass->model_mat = grass_mat;
		root->addChild(std::move(grass));


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
			glm::vec3(2.0f, 2.0f, -8.0f),    //pos
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