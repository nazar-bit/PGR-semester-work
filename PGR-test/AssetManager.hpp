#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include <glm/glm.hpp>

#include "pgr.h"
#include "Material.hpp"
#include "Mesh.hpp"


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

		
		void init();

		/// @brief 
		/// @param name Programer given name to the texture
		/// @param file_path Path to said texture
		void loadTetxure(const string& name, const string& file_path);
		
		/// @brief 
		/// @param name Programer given name to the texture
		/// @param file_path Path to said texture
		void loadTetxureNearest(const string & name, const string & file_path);

		/// @brief 
		/// @param name Programer given name to the material
		/// @param material Material&
		void loadMaterial(const string& name, const Material& material);

		/// @brief 
		/// @param name Programer given name to the mesh
		/// @param file_path Path to said mesh
		void loadMesh(const string& name, const string& file_path);

		/// @brief 
		/// @param name Programer given name to the mesh
		/// @param mesh unique ptr to Mesh
		void loadMesh(const string& name, std::unique_ptr<Mesh> mesh);


		/// @brief 
		/// @param name Programer given name to the texture
		/// @return 
		const GLuint getTexture(const string& name) const {
			return textures.at(name);
		}

		/// @brief 
		/// @param name Programer given name to the material
		/// @return 
		const Material* getMaterial(const string& name) const {
			return materials.at(name).get();
		}

		/// @brief 
		/// @param name Programer given name to the mesh
		/// @return 
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

		/// @brief 
		/// @param fileName Path to the texture
		/// @param mipmap create mipmap or not
		/// @return 
		GLuint createTexture(const std::string& fileName, bool mipmap = true);
	};





}