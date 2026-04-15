#pragma once

#include "pgr.h"
#include "Mesh.hpp"
#include "Material.hpp"
#include "AssetManager.hpp"
#include "Item.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp> 


namespace vasylnaz {

	class Object : public Item
	{
	public:
		const long object_id;


		Object(const string& mesh, const glm::mat4& model_matrix, const string& material = "basic",
			const string& dif_texture = "blank", const string& normal_map = "blank_norm")
			: object_id(global_object_id++), mesh(AssetManager::getInstance().getMesh(mesh)), model_matrix(model_matrix),
			material(AssetManager::getInstance().getMaterial(material)),
			dif_texture(AssetManager::getInstance().getTexture(dif_texture)),
			normal_map(AssetManager::getInstance().getTexture(normal_map)){
			// 
		}


		/// @brief 
		/// @param shader_manager 
		/// @param view 
		void draw(const ShaderManager& shader_manager, const glm::mat4& view) const;


		void updateItem(const glm::mat4& parent_model_matrix) override;


		const Mesh* get_mesh() const {
			return mesh;
		}


	private:
		static long global_object_id;
		const Mesh* mesh;
		const Material* material;
		GLuint dif_texture;
		GLuint normal_map;
		glm::mat4 model_matrix;
		glm::mat4 global_model_matrix;
	};





	

}