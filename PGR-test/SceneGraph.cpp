#include "SceneGraph.hpp"


namespace vasylnaz {

	long Node::global_node_id = 0;

	Node* Node::addChild(std::unique_ptr<Node> child) {
		Node* ptr = child.get();
		children.push_back(std::move(child));
		isDirty = true;
		return ptr;
	}



	void Node::addItem(std::unique_ptr<Item> item, RenderContext& render_context, bool render) {
		item->updateItem(global_model_mat);
		if (render)
		{
			if (Object* obj = dynamic_cast<Object*>(item.get())) {
				render_context.objects[obj->rq].push_back(obj);
			}
			else if (LightSource* ls = dynamic_cast<LightSource*>(item.get())) {
				render_context.light_block.addLight(ls);
			}
		}
		items.push_back(std::move(item));
	}


	void Node::renderItems(RenderContext& render_context) {
		for (auto& item : items) {
			if (Object* obj = dynamic_cast<Object*>(item.get())) {
				render_context.objects[obj->rq].push_back(obj);
			}
			else if (LightSource* ls = dynamic_cast<LightSource*>(item.get())) {
				render_context.light_block.addLight(ls);
			}
		}
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



	std::unique_ptr<Node> SceneGraph::loadOBJ(const std::string& filepath,
		const unsigned int from = 0, const unsigned int to = 10000, bool render) {

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
			auto msh = std::make_unique<Mesh>(mesh);
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

				node->addItem(std::move(object), render_context, render);
			}
		}

		return node;
	}



	void SceneGraph::findObject(long id, Actions action) {
		findObjectRecursive(id, action, root);
	}


	bool SceneGraph::findObjectRecursive(long id, Actions action, std::unique_ptr<Node>& node) {

		for (std::unique_ptr<Item>& item : node->items) {
			if (Object* obj = dynamic_cast<Object*>(item.get())) {
				if (obj->object_id == id) {
					if (action == Actions::CLICK) {
						obj->on_click(obj);
					}
					return true;
				}
			}
		}

		for (std::unique_ptr<Node>& child : node->children) {
			if (findObjectRecursive(id, action, child)) {
				return true;
			}
		}

		return false;
	}



	void addWallObjects(Node* wall, RenderContext& render_context, const glm::vec3& scale_vec) {
		static glm::mat4 bricks_mat = glm::mat4(1.0f);
		static glm::mat4 wall_support_mat = glm::mat4(1.0f);
		
		bricks_mat = glm::mat4(1.0f);
		bricks_mat = glm::scale(bricks_mat, glm::vec3(scale_vec));

		wall_support_mat = glm::mat4(1.0f);
		wall_support_mat = glm::translate(wall_support_mat, glm::vec3(0.0f, 0.001f, 0.5f));
		wall_support_mat = glm::scale(wall_support_mat, glm::vec3(scale_vec.x, scale_vec.y, 1.0f));
		

		// White bricks
		auto bricks = std::make_unique<Object>("ground", bricks_mat, "basic", "whitewashed_brick", "whitewashed_brick_norm");
		wall->addItem(std::move(bricks), render_context);

		// Wall support
		auto wall_support = std::make_unique<Object>("ground", wall_support_mat, "basic", "wall_support", "wall_support_norm");
		wall->addItem(std::move(wall_support), render_context);
	}


	void SceneGraph::addCubicalObjects(Node* cub) {
		static bool launched = false;
		static auto mac_mat = glm::mat4(1.0f);
		static auto desk_mat = glm::mat4(1.0f);
		static auto chair_mat = glm::mat4(1.0f);
		static auto hanging_light_mat = glm::mat4(1.0f);

		if (!launched) {
			launched = true;

			mac_mat = glm::translate(mac_mat, glm::vec3(0.1f, 1.0f, 1.4f));
			mac_mat = glm::rotate(mac_mat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			desk_mat = glm::translate(desk_mat, glm::vec3(0.0f, 0.0f, 0.6f));

			chair_mat = glm::translate(chair_mat, glm::vec3(0.2f, 0.28f, 1.0f));
			chair_mat = glm::rotate(chair_mat, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			hanging_light_mat = glm::translate(hanging_light_mat, glm::vec3(-1.0f, 3.46f, 0.5f));
			hanging_light_mat = glm::rotate(hanging_light_mat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		// MAC
		auto mac = loadOBJ("Models/mac_scaled.obj");
		auto screen = static_cast<Object*>(mac->items[0].get());
		screen->on_click = turnOffOnPC;
		mac->model_mat = mac_mat;
		cub->addChild(std::move(mac));

		// desk
		auto desk = loadOBJ("Models/office-desk_scaled.obj");
		desk->model_mat = desk_mat;
		cub->addChild(std::move(desk));

		// Chair
		auto chair = loadOBJ("Models/office_chair_modern_scaled.obj");
		chair->model_mat = chair_mat;
		cub->addChild(std::move(chair));

		// Hanging Light 
		auto hanging_light = loadOBJ("Models/caged_hanging_light_1k.gltf.obj");
		hanging_light->model_mat = hanging_light_mat;
		cub->addChild(std::move(hanging_light));
	}



	std::unique_ptr<Node> SceneGraph::createWindow(RenderContext& render_context, const glm::vec3& trans_vec) {
		//// Window 1
		auto window = loadOBJ("Models/window_scaled.obj", 5, 9, false);
		Object* obj = static_cast<Object*>(window->items[2].get());
		obj->material = AssetManager::getInstance().getMaterial("semi_trans");
		obj->rq = RenderQueue::TRANSPARENT_MASK;
		obj = static_cast<Object*>(window->items[4].get());
		obj->material = AssetManager::getInstance().getMaterial("semi_trans");
		obj->rq = RenderQueue::TRANSPARENT_MASK;
		auto window_mat = glm::mat4(1.0f);
		window->renderItems(render_context);
		window_mat = glm::translate(window_mat, trans_vec);
		window_mat = glm::rotate(window_mat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		window->model_mat = window_mat;
		return std::move(window);
	}



	void SceneGraph::init(ShaderProgram& shader_manager) {
		// 1
		
		// 2
		auto model_mat2 = glm::mat4(
			5.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 5.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 5.0f, 0.0f,
			15.0f, 0.0f, 4.0f, 1.0f);
		auto test_object2 = std::make_unique<Object>("cube", model_mat2, "basic", "blank", "blank_norm", "blank_em");
		root->addItem(std::move(test_object2), render_context);

		model_mat2 = glm::mat4(
			5.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 5.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 5.0f, 0.0f,
			5.0f, 0.0f, 25.0f, 1.0f);
		test_object2 = std::make_unique<Object>("cube", model_mat2, "basic", "blank", "blank_norm", "blank_em");
		root->addItem(std::move(test_object2), render_context);
		// 3
		auto model_mat3 = glm::mat4(
			100.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.1f, 0.0f, 0.0f,
			0.0f, 0.0f, 100.0f, 0.0f,
			0.0f, -3.0f, 0.0f, 1.0f);
		auto test_object3 = std::make_unique<Object>("cube", model_mat3, "red_plastic");

		
		glm::mat4 wall_support_mat = glm::mat4(1.0f);
		wall_support_mat = glm::translate(wall_support_mat, glm::vec3(0.0f, 0.001f, 0.5f));
		wall_support_mat = glm::scale(wall_support_mat, glm::vec3(10.0f, 1.0f, 1.0f));
		

		// Wall_1 ---|---
		auto wall = std::make_unique<Node>();
		auto wall_mat = glm::mat4(1.0f);
		wall_mat = glm::rotate(wall_mat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		wall->model_mat = wall_mat;
		addWallObjects(wall.get(), render_context, glm::vec3(10.0f, 1.0f, 5.0f));
		root->addChild(std::move(wall));

		


		// Wall_2 ---
		wall = std::make_unique<Node>();
		wall_mat = glm::mat4(1.0f);
		wall_mat = glm::translate(wall_mat, glm::vec3(-5.0f, 0.0f, 5.0f));
		wall_mat = glm::rotate(wall_mat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		wall_mat = glm::rotate(wall_mat, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		wall->model_mat = wall_mat;
		addWallObjects(wall.get(), render_context, glm::vec3(10.0f, 1.0f, 10.0f));
		root->addChild(std::move(wall));


		// Wall_3 ---
		wall = std::make_unique<Node>();
		wall_mat = glm::mat4(1.0f);
		wall_mat = glm::translate(wall_mat, glm::vec3(0.0f, 0.0f, 10.0f));
		wall_mat = glm::rotate(wall_mat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		wall_mat = glm::rotate(wall_mat, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		wall->model_mat = wall_mat;

		// White bricks
		glm::mat4 special = glm::mat4(1.0f);
		special = glm::translate(special, glm::vec3(-2.0f, 0.0f, 0.0f));
		auto bricks = std::make_unique<Object>("wall_with_holes", special, "basic", "whitewashed_brick", "whitewashed_brick_norm");
		wall->addItem(std::move(bricks), render_context);

		// Wall support
		auto wall_support = std::make_unique<Object>("ground", wall_support_mat, "basic", "wall_support", "wall_support_norm");
		wall->addItem(std::move(wall_support), render_context);

		//// Window 1
		auto window = createWindow(render_context, glm::vec3(-2.9f, 0.0f, -0.2f));
		wall->addChild(std::move(window));

		//// Window 2
		window = createWindow(render_context, glm::vec3(0.1f, 0.0f, -0.2f));
		wall->addChild(std::move(window));

		// Window 3
		window = createWindow(render_context, glm::vec3(3.1f, 0.0f, -0.2f));
		wall->addChild(std::move(window));

		root->addChild(std::move(wall));


		// Wall_4 ---
		wall = std::make_unique<Node>();
		wall_mat = glm::mat4(1.0f);
		wall_mat = glm::translate(wall_mat, glm::vec3(5.0f, 0.0f, 5.0f));
		wall_mat = glm::rotate(wall_mat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		wall_mat = glm::rotate(wall_mat, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		wall->model_mat = wall_mat;

		// White bricks
		bricks = std::make_unique<Object>("wall_with_holes", special, "basic", "whitewashed_brick", "whitewashed_brick_norm");
		wall->addItem(std::move(bricks), render_context);

		// Wall support
		wall_support = std::make_unique<Object>("ground", wall_support_mat, "basic", "wall_support", "wall_support_norm");
		wall->addItem(std::move(wall_support), render_context);

		// Window 4
		window = createWindow(render_context, glm::vec3(0.1f, 0.0f, -0.2f));
		wall->addChild(std::move(window));

		// Window 5
		window = createWindow(render_context, glm::vec3(3.1f, 0.0f, -0.2f));
		wall->addChild(std::move(window));

		root->addChild(std::move(wall));


		// Wall_5 ---
		wall = std::make_unique<Node>();
		wall_mat = glm::mat4(1.0f);
		wall_mat = glm::translate(wall_mat, glm::vec3(3.5f, 0.0f, 3.0f));
		wall_mat = glm::rotate(wall_mat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		wall->model_mat = wall_mat;

		addWallObjects(wall.get(), render_context, glm::vec3(3.0f, 1.0f, 5.0f));
		root->addChild(std::move(wall));


		// Wall_6 ---
		wall = std::make_unique<Node>();
		wall_mat = glm::mat4(1.0f);
		wall_mat = glm::translate(wall_mat, glm::vec3(2.0f, 0.0f, 1.5f));
		wall_mat = glm::rotate(wall_mat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		wall_mat = glm::rotate(wall_mat, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		wall->model_mat = wall_mat;

		addWallObjects(wall.get(), render_context, glm::vec3(3.0f, 1.0f, 5.0f));
		root->addChild(std::move(wall));



		//floor ---|---
		auto floor_mat = glm::mat4(
			10.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 10.0f, 0.0f,
			0.0f, -0.7f, 5.0f, 1.0f);
		auto floor_obj = std::make_unique<Object>("ground", floor_mat, "basic", "floor", "floor_norm");
		root->addItem(std::move(floor_obj), render_context);

		//black ceiling  ---|---
		auto ceiling_mat = glm::mat4(1.0f);
		ceiling_mat = glm::translate(ceiling_mat, glm::vec3(0.0f, 2.5f, 5.0f));
		ceiling_mat = glm::rotate(ceiling_mat, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		ceiling_mat = glm::scale(ceiling_mat, glm::vec3(10.0f, 1.0f, 10.0f));
		auto ceiling_obj = std::make_unique<Object>("ground", ceiling_mat, "basic", "ceiling", "ceiling_norm");
		root->addItem(std::move(ceiling_obj), render_context);



		// Cubical 1 ---|---
		auto cub = std::make_unique<Node>();
		auto cub_mat = glm::mat4(1.0f);
		cub_mat = glm::translate(cub_mat, glm::vec3(0.5f, -0.96f, 2.5f));
		cub_mat = glm::rotate(cub_mat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cub->model_mat = cub_mat;

		addCubicalObjects(cub.get());
		
		root->addChild(std::move(cub));


		// Cubical 2 ---
		cub = std::make_unique<Node>();
		cub_mat = glm::mat4(1.0f);
		cub_mat = glm::translate(cub_mat, glm::vec3(-2.0f, -0.96f, 2.5f));
		cub_mat = glm::rotate(cub_mat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cub->model_mat = cub_mat;

		addCubicalObjects(cub.get());

		root->addChild(std::move(cub));


		// Cubical 3 ---
		cub = std::make_unique<Node>();
		cub_mat = glm::mat4(1.0f);
		cub_mat = glm::translate(cub_mat, glm::vec3(-2.0f, -0.96f, 5.5f));
		cub_mat = glm::rotate(cub_mat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cub->model_mat = cub_mat;

		addCubicalObjects(cub.get());

		root->addChild(std::move(cub));


		// Cubical 4 ---
		cub = std::make_unique<Node>();
		cub_mat = glm::mat4(1.0f);
		cub_mat = glm::translate(cub_mat, glm::vec3(0.5f, -0.96f, 5.5f));
		cub_mat = glm::rotate(cub_mat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cub->model_mat = cub_mat;

		addCubicalObjects(cub.get());

		root->addChild(std::move(cub));


		// Cubical 5 ---
		cub = std::make_unique<Node>();
		cub_mat = glm::mat4(1.0f);
		cub_mat = glm::translate(cub_mat, glm::vec3(3.0f, -0.96f, 5.5f));
		cub_mat = glm::rotate(cub_mat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cub->model_mat = cub_mat;

		addCubicalObjects(cub.get());

		// Thermos
		auto thermos_mat = glm::mat4(1.0f);
		thermos_mat = glm::translate(thermos_mat, glm::vec3(-0.4f, 1.0f, 0.7f));
		auto thermos = std::make_unique<Object>("thermos", thermos_mat, "basic", "thermos", "thermos_norm");
		cub->addItem(std::move(thermos), render_context);

		root->addChild(std::move(cub));



		// Cubical 6 ---
		cub = std::make_unique<Node>();
		cub_mat = glm::mat4(1.0f);
		cub_mat = glm::translate(cub_mat, glm::vec3(-2.0f, -0.96f, 8.5f));
		cub_mat = glm::rotate(cub_mat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cub->model_mat = cub_mat;

		addCubicalObjects(cub.get());

		root->addChild(std::move(cub));



		// Cubical 7 ---
		cub = std::make_unique<Node>();
		cub_mat = glm::mat4(1.0f);
		cub_mat = glm::translate(cub_mat, glm::vec3(0.5f, -0.96f, 8.5f));
		cub_mat = glm::rotate(cub_mat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cub->model_mat = cub_mat;

		addCubicalObjects(cub.get());

		root->addChild(std::move(cub));




		// Cubical 8 ---
		cub = std::make_unique<Node>();
		cub_mat = glm::mat4(1.0f);
		cub_mat = glm::translate(cub_mat, glm::vec3(3.0f, -0.96f, 8.5f));
		cub_mat = glm::rotate(cub_mat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cub->model_mat = cub_mat;

		addCubicalObjects(cub.get());

		root->addChild(std::move(cub));

		
		

		


		// Skybox --|--
		auto skybox = loadOBJ("Models/skybox.obj", 0, 10000, false);
		for (auto& item : skybox->items) {
			auto skybox_obj = static_cast<Object*>(item.get());
			skybox_obj->rq = RenderQueue::SKYBOX;
		}
		skybox->renderItems(render_context);
		auto skybox_mat = glm::mat4(1.0f);
		skybox_mat = glm::scale(skybox_mat, glm::vec3(50.0f));
		skybox->model_mat = skybox_mat;
		root->addChild(std::move(skybox));
		

		
		

		
		


		auto testLight = std::make_unique<LightSource>(
			DIRECTIONAL,
			glm::vec3(0.1f),     //amb
			glm::vec3(0.9f),     //diff
			glm::vec3(0.2f),    //spec
			glm::vec3(0.2f, 1.0f, 1.0f)   //pos
		);
		root->addItem(std::move(testLight), render_context);


		//auto testLight2 = std::make_unique<LightSource>(
		//	DIRECTIONAL,
		//	glm::vec3(0.1f),     //amb
		//	glm::vec3(0.1f),     //diff
		//	glm::vec3(0.2f),    //spec
		//	glm::vec3(0.0f, -1.0f, 1.0f)   //pos
		//);
		//root->addItem(std::move(testLight2), render_context);


		// Room point
		auto point_lights_node = std::make_unique<Node>();
		auto point_light = std::make_unique<LightSource>(
			POINT,
			glm::vec3(0.1f),     //amb
			glm::vec3(0.1f),     //diff
			glm::vec3(0.0f),    //spec
			glm::vec3(1.0f, 0.0f, 3.0f),    //pos
			Attenuation(1.0f, 0.01f, 0.0017f),
			Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f)
		);
		point_lights_node->addItem(std::move(point_light), render_context);


		point_light = std::make_unique<LightSource>(
			POINT,
			glm::vec3(0.1f),     //amb
			glm::vec3(0.1f),     //diff
			glm::vec3(0.0f),    //spec
			glm::vec3(-2.0f, 0.0f, 3.0f),    //pos
			Attenuation(1.0f, 0.01f, 0.0017f),
			Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f)
		);
		point_lights_node->addItem(std::move(point_light), render_context);


		point_light = std::make_unique<LightSource>(
			POINT,
			glm::vec3(0.1f),     //amb
			glm::vec3(0.1f),     //diff
			glm::vec3(0.0f),    //spec
			glm::vec3(-2.0f, 0.0f, 7.0f),    //pos
			Attenuation(1.0f, 0.01f, 0.0017f),
			Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f)
		);
		point_lights_node->addItem(std::move(point_light), render_context);


		point_light = std::make_unique<LightSource>(
			POINT,
			glm::vec3(0.1f),     //amb
			glm::vec3(0.1f),     //diff
			glm::vec3(0.0f),    //spec
			glm::vec3(4.0f, 0.0f, 7.0f),    //pos
			Attenuation(1.0f, 0.01f, 0.0017f),
			Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f)
		);
		point_lights_node->addItem(std::move(point_light), render_context);
		root->addChild(std::move(point_lights_node));


		//auto testLight2 = std::make_unique<LightSource>(
		//	POINT,
		//	glm::vec3(0.1f),     //amb
		//	glm::vec3(2.0f),     //diff
		//	glm::vec3(1.0f),    //spec
		//	glm::vec3(6.0f, 2.0f, 5.0f),    //pos
		//	Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f),
		//	Attenuation(1.0f, 0.09f, 0.032f)
		//);


		//auto testLight3 = std::make_unique<LightSource>(
		//	POINT,
		//	glm::vec3(0.1f),     //amb
		//	glm::vec3(2.0f),     //diff
		//	glm::vec3(1.0f),    //spec
		//	glm::vec3(-5.0f, 0.5f, 15.0f),    //pos
		//	Spotlight(glm::vec3(0.0f, 0.0f, -1.0f), 45.0f, 1.0f),
		//	Attenuation(1.0f, 0.09f, 0.032f)
		//);



		/*const LightSource testLight2(
			glm::vec3(0.1f),
			glm::vec3(0.8f),
			glm::vec3(1.0f),
			glm::vec3(0.0f, 15.0f, 0.0f),
			1.0f,
			1.0f
		);*/


		
		/*floor_node->addItem(std::move(testLight2), render_context);
		floor_node->addItem(std::move(testLight3), render_context);*/
	}
}