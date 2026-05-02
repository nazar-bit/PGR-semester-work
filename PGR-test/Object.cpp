#include "Object.hpp"


namespace vasylnaz {

	long Object::global_object_id = 0;


	void Object::draw(const ShaderProgram& shader_program, const glm::mat4& view) const {
		
		shader_program.loadMaterial(*material);
		shader_program.loadDiffuse(dif_texture);
		shader_program.loadNormal(normal_map);	
		shader_program.loadEmission(em_map);
		shader_program.loadId(object_id);
	
		mesh->draw(shader_program, global_model_matrix, view);
	}


	void Object::updateItem(const glm::mat4& parent_model_matrix) {
		global_model_matrix = parent_model_matrix * model_matrix;
	}


	void TextObject::draw(const ShaderProgram& shader_program, const glm::mat4& view) const {

		shader_program.loadMaterial(*material);
		shader_program.loadDiffuse(dif_texture);
		shader_program.loadNormal(normal_map);
		shader_program.loadEmission(em_map);
		shader_program.loadId(object_id);
		shader_program.loadTextColor(text_color);

		mesh->draw(shader_program, global_model_matrix, view);
	}
}


