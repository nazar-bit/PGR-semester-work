#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include <glm/glm.hpp>

#include "pgr.h"
#include "Material.hpp"
#include "Mesh.hpp"
#include "ShaderManager.hpp"

using std::string;


namespace vasylnaz {
	class AssetManager {
	public:

		GLint diffuse_map_location;


		AssetManager()
		{
			//
		}


		void init(ShaderManager& shader_manager);

		
		void loadTetxure(const string& name, const string& file_path) {
			auto it = textures.find(name);

			if (it != textures.end()) {
				std::cout << "Texture was already loaded" << "\n";
			}
			else {
				GLuint texID = pgr::createTexture(file_path);
				textures.emplace(name, texID);
				std::cout << "Successfully loaded: " << file_path << std::endl;
			}
		}


		void loadMaterial(const string& name, const Material& material) {
			auto it = materials.find(name);

			if (it != materials.end()) {
				std::cout << "Material was already loaded" << "\n";
			}
			else {
				materials.emplace(name, std::make_unique<Material>(material));
				std::cout << "Successfully loaded: " << name << std::endl;
			}
		}


		void loadMesh(const string& name, const string& file_path, ShaderManager& shader_manager) {
			auto it = meshes.find(name);

			if (it != meshes.end()) {
				std::cout << "Mesh was already loaded" << "\n";
			}
			else {
				meshes.emplace(name, std::make_unique<Mesh>(file_path, shader_manager));
			}
		}


		void loadMesh(const string& name, std::unique_ptr<Mesh> mesh) {
			auto it = meshes.find(name);

			if (it != meshes.end()) {
				std::cout << "Mesh was already loaded" << "\n";
			}
			else {
				meshes.emplace(name, std::move(mesh));
			}
		}


		const GLuint getTexture(const string& name) const {
			return textures.at(name);
		}


		const Material* getMaterial(const string& name) const {
			return materials.at(name).get();
		}


		const Mesh* getMesh(const string& name) const {
			return meshes.at(name).get();
		}


	private:
		std::unordered_map<string, GLuint> textures;
		std::unordered_map<string, std::unique_ptr<Material>> materials;
		std::unordered_map<string, std::unique_ptr<Mesh>> meshes;
	};





}