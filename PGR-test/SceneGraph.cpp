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



	std::unique_ptr<Node> SceneGraph::loadOBJ(const std::string& filepath, ShaderManager& shader_manager,
		const unsigned int from = 0, const unsigned int to = 10000) {

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
			if (!(i >= from && i <= to)) continue;
			aiMesh* mesh = scene->mMeshes[i];

			auto mesh_name = filepath + mesh->mName.C_Str();
			auto msh = std::make_unique<Mesh>(mesh, shader_manager);
			AssetManager::getInstance().loadMesh(mesh_name, std::move(msh));

			unsigned int materialIndex = mesh->mMaterialIndex;

			if (materialIndex >= 0 && materialIndex < scene->mNumMaterials) {
				aiMaterial* material = scene->mMaterials[materialIndex];
				aiString texturePath;
				//aiString materialName;
				std::string dif_tex = "blank";
				std::string norm_map = "blank_norm";
				std::string em_map = "blank_em";
				// Get texture name
				/*if (material->Get(AI_MATKEY_NAME, materialName) == AI_SUCCESS) {
					std::cout << "Object " << mesh_name
						<< " uses Texture: " << materialName.C_Str() << std::endl;
				}
				else {
					std::cout << "Object " << mesh_name << " uses an unnamed material." << std::endl;
					hasDifTex = false;
				}*/


				// Get Diffuse texture
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {

					std::cout << "Object " << mesh_name
						<< " uses Diffuse Texture: " << texturePath.C_Str() << std::endl;
					AssetManager::getInstance().loadTetxure(texturePath.C_Str(), texturePath.C_Str());
					dif_tex = texturePath.C_Str();
				}
				else {
					std::cout << "Object " << mesh_name << " has no diffuse texture." << std::endl;
				}

				// Get Normal map
				if (material->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS) {

					std::cout << "Object " << mesh_name
						<< " uses Normal map: " << texturePath.C_Str() << std::endl;
					AssetManager::getInstance().loadTetxure(texturePath.C_Str(), texturePath.C_Str());
					norm_map = texturePath.C_Str();
				}
				else if (material->GetTexture(aiTextureType_HEIGHT, 0, &texturePath) == AI_SUCCESS) {
					std::cout << "Object " << mesh_name << " uses Normal map (loaded from Height/Bump slot): " << texturePath.C_Str() << std::endl;
					AssetManager::getInstance().loadTetxure(texturePath.C_Str(), texturePath.C_Str());
					norm_map = texturePath.C_Str();
				}
				else {
					std::cout << "Object " << mesh_name << " has no normal map." << std::endl;
				}

				// Get Emission map
				if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0) {

					if (material->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath) == AI_SUCCESS) {
						std::cout << "Found Emissive Map: " << texturePath.C_Str() << '\n';
						AssetManager::getInstance().loadTetxure(texturePath.C_Str(), texturePath.C_Str());
						em_map = texturePath.C_Str();
					}
				}
				else {
					std::cout << "Object " << mesh_name << " has no emission map." << std::endl;
				}

				auto object = std::make_unique<Object>(mesh_name, glm::mat4(1.0f),
					"basic", dif_tex, norm_map, em_map);

				node->addItem(std::move(object), render_context);
			}
		}

		return node;
	}



	void SceneGraph::init(ShaderManager& shader_manager) {
		// 1
		auto test_object = std::make_unique<Object>("thermos", glm::mat4(1.0f), "basic", "thermos", "thermos_norm");
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
		auto ground_mat = glm::mat4(
			10.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 10.0f, 0.0f,
			0.0f, -0.8f, 0.0f, 1.0f);
		auto ground_obj = std::make_unique<Object>("ground", ground_mat, "basic", "rocks", "rocks_norm");
		root->addItem(std::move(ground_obj), render_context);

		root->addItem(std::move(test_object), render_context);
		root->addItem(std::move(test_object2), render_context);
		auto floor_node = root->addChild(std::move(floor));
		floor_node->addItem(std::move(test_object3), render_context);
		// Grass
		/*auto grass = loadOBJ("Models/searsia_lucida_1k.obj", shader_manager, 3, 5);
		auto grass_mat = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, -0.8f, 0.0f, 1.0f);
		grass->model_mat = grass_mat;
		root->addChild(std::move(grass));*/
		// Table
		auto table = loadOBJ("Models/wooden_picnic_table_1k.gltf.obj", shader_manager);
		auto table_mat = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, -0.8f, 0.0f, 1.0f);
		table->model_mat = table_mat;
		root->addChild(std::move(table));

		// Stone fire
		auto sfire = loadOBJ("Models/stone_fire_pit_1k.gltf.obj", shader_manager);
		auto sfire_mat = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-2.0f, -0.7f, 0.0f, 1.0f);
		sfire->model_mat = sfire_mat;
		root->addChild(std::move(sfire));

		//// Fir
		//auto fir = loadOBJ("Models/fir_sapling_medium_1k.gltf.obj", shader_manager, 6, 8);
		//auto fir_mat = glm::mat4(
		//	1.0f, 0.0f, 0.0f, 0.0f,
		//	0.0f, 1.0f, 0.0f, 0.0f,
		//	0.0f, 0.0f, 1.0f, 0.0f,
		//	-10.0f, -0.7f, 0.0f, 1.0f);
		//fir->model_mat = fir_mat;
		//root->addChild(std::move(fir));


		// White bricks
		glm::mat4 bricks_mat = glm::mat4(1.0f);
		bricks_mat = glm::rotate(bricks_mat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		bricks_mat = glm::scale(bricks_mat, glm::vec3(10.0f, 1.0f, 10.0f));
		auto bricks = std::make_unique<Object>("ground", bricks_mat, "basic", "whitewashed_brick", "whitewashed_brick_norm");
		root->addItem(std::move(bricks), render_context);


		// Wooden gates
		glm::mat4 gates_mat = glm::mat4(1.0f);
		gates_mat = glm::translate(gates_mat, glm::vec3(0.0f, -0.5f, 0.1f));
		gates_mat = glm::rotate(gates_mat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		gates_mat = glm::scale(gates_mat, glm::vec3(10.0f, 1.0f, 1.0f));
		auto gates = std::make_unique<Object>("ground", gates_mat, "basic", "wooden_gate", "wooden_gate_norm");
		root->addItem(std::move(gates), render_context);


		//black planks 
		auto planks_mat = glm::mat4(
			10.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 10.0f, 0.0f,
			0.0f, -0.7f, 0.0f, 1.0f);
		auto planks_obj = std::make_unique<Object>("ground", planks_mat, "basic", "black_painted_planks", "black_painted_planks_norm");
		root->addItem(std::move(planks_obj), render_context);

		// MAC
		auto mac = loadOBJ("Models/mac_scaled.obj", shader_manager);
		auto mac_mat = glm::mat4(1.0f);
		mac_mat = glm::translate(mac_mat, glm::vec3(0, 0, 0.6));
		//mac_mat = glm::scale(mac_mat, glm::vec3(0.01, 0.01, 0.01));
		mac->model_mat = mac_mat;

		//Object* obj = static_cast<Object*>(mac->items[2].get());
		//obj->model_matrix = glm::translate(obj->model_matrix, glm::vec3(1, 0, 0));
		root->addChild(std::move(mac));

		
		


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