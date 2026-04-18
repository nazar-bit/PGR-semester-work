#include "Object.hpp"


namespace vasylnaz {

	void doNothing(Object* obj) {
		return;
	}


	void turnOffOnPC(Object* obj) {
		static GLuint pc_on_map = obj->em_map;
		static GLuint pc_off_map = AssetManager::getInstance().getTexture("blank_em");
		if (obj->em_map == pc_off_map) {
			obj->em_map = pc_on_map;
		}
		else {
			obj->em_map = pc_off_map;
		}
	}


	long Object::global_object_id = 0;


	void Object::draw(const ShaderProgram& shader_manager, const glm::mat4& view) const {
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


	void Object::pickRender(const PickingProgram& pick_prog, const glm::mat4& view) const {
		glUniform1f(pick_prog.positionId, (float)object_id / 255.0f);
		mesh->pickRender(pick_prog, global_model_matrix, view);
	}


}