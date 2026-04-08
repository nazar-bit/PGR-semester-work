#include "Object.hpp"


namespace vasylnaz {

	void Object::draw(const ShaderManager& shader_manager, const glm::mat4& view) const {
		shader_manager.change_material(*material);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, dif_texture);
		glUniform1i(shader_manager.positionDiffuseMap, 0);
		mesh->draw(shader_manager, model_matrix, view);
	}


}