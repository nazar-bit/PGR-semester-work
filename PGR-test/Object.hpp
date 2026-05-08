#pragma once

#include "pgr.h"
#include "Mesh.hpp"
#include "Material.hpp"
#include "AssetManager.hpp"
#include "Item.hpp"
#include "Script.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace vasylnaz {


	class Script;


	class Object : public Item
	{
	public:
		const long object_id;
		glm::mat4 model_matrix;
		const Material* material;
		RenderQueue rq;
		std::vector<std::unique_ptr<Script>> scripts;
		GLuint em_map;
		bool clicked = false;

		/// @brief 
		/// @param mesh Programer given name to the mesh
		/// @param model_matrix Model matrix
		/// @param material Programer given name to the material
		/// @param dif_texture Programer given name to diffuse texture
		/// @param normal_map Programer given name to normal map
		/// @param em_map Programer given name to emission map
		/// @param rq RenderQueue (F.E. OPAQUE_MASK or TRANSPARENT_MASK)
		Object(const string& mesh, const glm::mat4& model_matrix, const string& material = "basic",
			const string& dif_texture = "blank", const string& normal_map = "blank_norm",
			const string& em_map = "blank_em", const RenderQueue rq = RenderQueue::OPAQUE_MASK)
			: object_id(global_object_id++), mesh(AssetManager::getInstance().getMesh(mesh)), model_matrix(model_matrix),
			material(AssetManager::getInstance().getMaterial(material)),
			dif_texture(AssetManager::getInstance().getTexture(dif_texture)),
			normal_map(AssetManager::getInstance().getTexture(normal_map)),
			em_map(AssetManager::getInstance().getTexture(em_map)),
			rq(rq)
			{
				// 
			}


		/// @brief 
		/// @param shader_program ShaderProgram&
		/// @param view View matrix
		virtual void draw(const ShaderProgram& shader_program, const glm::mat4& view) const;

		/// @brief Update the object based on the Parent's model matrix in SceneGraph 
		/// @param parent_model_matrix Parent's model matrix 
		void updateItem(const glm::mat4& parent_model_matrix) override;

		/// @brief 
		/// @return Pointer to Mesh not the Programer givem name
		const Mesh* getMesh() const {
			return mesh;
		}


	protected:
		static long global_object_id;
		const Mesh* mesh;
		GLuint dif_texture;
		GLuint normal_map;
		glm::mat4 global_model_matrix;
	};



	
	class TextObject : public Object {
	public:
		glm::vec3 text_color;

		/// @brief 
		/// @param mesh Programer given name to the mesh
		/// @param model_matrix Model matrix
		/// @param material Programer given name to the material
		/// @param dif_texture Programer given name to diffuse texture
		/// @param normal_map Programer given name to normal map
		/// @param em_map Programer given name to emission map
		/// @param rq RenderQueue (F.E. OPAQUE_MASK or TRANSPARENT_MASK)
		TextObject(const string& mesh, const glm::mat4& model_matrix, const string& material = "basic",
			const string& dif_texture = "blank", const string& normal_map = "blank_norm",
			const string& em_map = "blank_em", const RenderQueue rq = RenderQueue::OPAQUE_MASK)
			: Object(mesh, model_matrix, material, dif_texture, normal_map, em_map, rq)
		{
			// 
		}

		/// @brief 
		/// @param shader_program ShaderProgram&
		/// @param view View matrix
		void draw(const ShaderProgram& shader_program, const glm::mat4& view) const;

	private:

	};
}