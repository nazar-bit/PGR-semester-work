#pragma once

#include "pgr.h"
#include "Mesh.hpp"
#include "Material.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp> 


namespace vasylnaz {

	class Object
	{
	public:
		const long object_id;
		Material material;


		Object(const Mesh* mesh, const glm::mat4& model_matrix, Material& material)
			: object_id(global_object_id++), mesh(mesh), model_matrix(model_matrix),
			material(material){
			// 
		}


		void draw(const ShaderManager& shader_manager, const glm::mat4& view) const {
			shader_manager.change_material(material);
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