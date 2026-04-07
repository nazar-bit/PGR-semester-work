#pragma once

#include "pgr.h"
#include "Mesh.hpp"
#include "Material.hpp"
#include "TextureManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp> 


namespace vasylnaz {

	class Object
	{
	public:
		const long object_id;
		Material material;
		string dif_texture;


		Object(const Mesh* mesh, const glm::mat4& model_matrix, Material& material,
			const string& dif_texture)
			: object_id(global_object_id++), mesh(mesh), model_matrix(model_matrix),
			material(material), dif_texture(dif_texture){
			// 
		}


		void draw(const ShaderManager& shader_manager, const TextureManager& texture_manager,
			const glm::mat4& view) const {
			shader_manager.change_material(material);
			texture_manager.feed_diffuse_map(dif_texture);
			mesh->draw(shader_manager, model_matrix, view);
		}


		const Mesh* get_mesh() const {
			return mesh;
		}


	private:
		static long global_object_id;
		const Mesh* mesh;
		glm::mat4 model_matrix;
	};





	

}