#include "Object.hpp"


namespace vasylnaz {

	void Object::draw(const ShaderManager& shader_manager, const glm::mat4& view) const {
		// material
		shader_manager.change_material(*material);
		// diffuseTex
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, dif_texture);
		glUniform1i(shader_manager.positionDiffuseMap, 0);
		// NormMap
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal_map);
		glUniform1i(shader_manager.positionNormalMap, 1);
		// EmMap
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, em_map);
		glUniform1i(shader_manager.positionEmmisionMap, 2);
		// Draw
		mesh->draw(shader_manager, global_model_matrix, view);
	}


	void Object::updateItem(const glm::mat4& parent_model_matrix) {
		global_model_matrix = parent_model_matrix * model_matrix;
	}


}