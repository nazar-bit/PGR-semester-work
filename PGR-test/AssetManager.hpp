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

		static AssetManager& getInstance() {
			static AssetManager AM;
			return AM;
		}


		AssetManager(const AssetManager&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;

		AssetManager(AssetManager&&) = delete;
		AssetManager& operator=(AssetManager&&) = delete;

		
		void init(ShaderProgram& shader_manager);

		/// @brief 
		/// @param name 
		/// @param file_path 
		void loadTetxure(const string& name, const string& file_path);

		/// @brief 
		/// @param name 
		/// @param material 
		void loadMaterial(const string& name, const Material& material);

		/// @brief 
		/// @param name 
		/// @param file_path 
		/// @param shader_manager 
		void loadMesh(const string& name, const string& file_path);

		/// @brief 
		/// @param name 
		/// @param mesh 
		void loadMesh(const string& name, std::unique_ptr<Mesh> mesh);


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

		AssetManager()
		{
			//
		}
	};





}