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



	void SceneGraph::updateRecursive(std::unique_ptr<Node>& node,
		const glm::mat4& parent_glob_mat, bool dirty) {

		for (std::unique_ptr<Item>& item : node->items) {
			if (Object* obj = dynamic_cast<Object*>(item.get())) {
				for (auto& script : obj->scripts) {
					script->update();
				}
			}
		}

		if (node->scripts.size() != 0) {
			for (auto& script : node->scripts) {
				script->update();
			}
			dirty = true;
		}
		

		if (dirty || node->isDirty) {
			node->update(parent_glob_mat);
			for (std::unique_ptr<Item>& item : node->items) {
				item->updateItem(node->global_model_mat);
			}
			node->isDirty = false;
			dirty = true;
		}

		for (std::unique_ptr<Node>& child : node->children) {
			updateRecursive(child, node->global_model_mat, dirty);
		}
	}



	void SceneGraph::findObject(long id, Actions action) {
		findObjectRecursive(id, action, root);
	}


	bool SceneGraph::findObjectRecursive(long id, Actions action, std::unique_ptr<Node>& node) {
		for (std::unique_ptr<Item>& item : node->items) {
			if (Object* obj = dynamic_cast<Object*>(item.get())) {
				if (obj->object_id == id) {
					if (action == Actions::CLICK) {
						if (obj->clicked) {
							obj->clicked = false;
						}
						else {
							obj->clicked = true;
						}
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



	std::unique_ptr<Node> SceneGraph::loadOBJ(const std::string& filepath,
		const unsigned int from, const unsigned int to, bool render) {

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
		std::cout << "----Successfully loaded: " << filepath << std::endl;
		std::cout << "----Total meshes found: " << scene->mNumMeshes << std::endl;


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



	std::unique_ptr<Node> SceneGraph::createText(const std::string& text, const unsigned int font_size,
		const glm::vec3& font_color) 
	{
		auto node = std::make_unique<Node>();
		const int vec_size = 3;
		const int tex_vec_size = 2;
		const int vec_num = 4;
		const int indices_num = 2;

		const float positions[vec_size * vec_num] =
		{
			0.0f, 0.0f, 0.0f,
			(font_size * CHAR_W) * FONT_SCALE_FACTOR, 0.0f, 0.0f,
			0.0f, (font_size * CHAR_H) * FONT_SCALE_FACTOR, 0.0f,
			(font_size * CHAR_W) * FONT_SCALE_FACTOR, (font_size * CHAR_H) * FONT_SCALE_FACTOR, 0.0f,
		};

		const unsigned int indices[indices_num * vec_size] =
		{
			0, 1, 2,
			1, 3, 2,
		};
		

		float x = CHAR_PADDING * FONT_SCALE_FACTOR;
		float y = CHAR_PADDING * FONT_SCALE_FACTOR;
		for (unsigned long char_idx = 0; char_idx < text.size(); ++char_idx) {
			
			char ch = text[char_idx];
			if (ch == '\n') {
				y -= (CHAR_PADDING + CHAR_H * font_size) * FONT_SCALE_FACTOR;
				x = CHAR_PADDING * FONT_SCALE_FACTOR;
				continue;
			}

			char relative_ch = ch - FIRST_CHAR_ID;
			int relative_x = relative_ch % N_CHAR_IN_ROW;
			int relative_y = (TOTAL_ROWS - 1) - (relative_ch / N_CHAR_IN_ROW);

			float tex_x = (float)(relative_x * (2 * CHAR_PADDING + CHAR_W)) / FONT_W;
			float tex_y = (float)(relative_y * (2 * CHAR_PADDING + CHAR_H)) / FONT_H;
			tex_y += 1.0f / FONT_H; //offset
			float norm_w = (float)(CHAR_W + 2*CHAR_PADDING) / FONT_W;
			float norm_h = (float)(CHAR_H + 2*CHAR_PADDING) / FONT_H;

			float texture_coords[vec_num * tex_vec_size] =
			{
				tex_x, tex_y,                   
				tex_x + norm_w, tex_y,          
				tex_x, tex_y + norm_h,          
				tex_x + norm_w, tex_y + norm_h, 
			};
			
			auto mesh = std::make_unique<Mesh>(positions, vec_num, texture_coords, indices, indices_num);
			const std::string mesh_name = std::string("letter") + ch;
			AssetManager::getInstance().loadMesh(mesh_name, std::move(mesh));

			auto obj_mat = glm::mat4(1.0f);
			obj_mat = glm::translate(obj_mat, glm::vec3(x, y, 0));

			auto obj = std::make_unique<TextObject>(mesh_name, obj_mat, "basic", "font", "blank_norm", "blank_em", RenderQueue::TEXT);
			obj->text_color = font_color;
			node->addItem(std::move(obj), render_context);

			x += (CHAR_PADDING + CHAR_W * font_size) * FONT_SCALE_FACTOR;
		}

		return std::move(node);
	}
}