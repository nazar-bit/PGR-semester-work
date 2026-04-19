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
}