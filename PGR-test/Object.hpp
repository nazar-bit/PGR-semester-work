#pragma once

#include "pgr.h"
#include "Mesh.hpp"
#include "Material.hpp"
#include "AssetManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp> 


namespace vasylnaz {

	class Object
	{
	public:
		const long object_id;


		Object(const AssetManager& asset_manager, const string& mesh, const glm::mat4& model_matrix,
			const string& material = "basic", const string& dif_texture = "blank")
			: object_id(global_object_id++), mesh(asset_manager.getMesh(mesh)), model_matrix(model_matrix),
			material(asset_manager.getMaterial(material)), dif_texture(asset_manager.getTexture(dif_texture)){
			// 
		}


		/// @brief 
		/// @param shader_manager 
		/// @param view 
		void draw(const ShaderManager& shader_manager, const glm::mat4& view) const;


		const Mesh* get_mesh() const {
			return mesh;
		}


	private:
		static long global_object_id;
		const Mesh* mesh;
		const Material* material;
		GLuint dif_texture;
		glm::mat4 model_matrix;
	};





	

}