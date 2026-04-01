#pragma once

#include "pgr.h"
#include "ShaderManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 


namespace vasylnaz {
	class Mesh
	{
	public:
		const long mesh_id;

		Mesh(const float* vertices_pos, const long vertices_count, const float* normals,
			const unsigned short* vertices_indices, const long indices_count,
			ShaderManager shader_manager)

			: mesh_id(global_mesh_id++), indices_count(indices_count) {

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(float), vertices_pos, GL_STATIC_DRAW);
			glEnableVertexAttribArray(shader_manager.positionLoc);
			glVertexAttribPointer(shader_manager.positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glGenBuffers(1, &normals_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(float), normals, GL_STATIC_DRAW);
			glEnableVertexAttribArray(shader_manager.positionNormal);
			glVertexAttribPointer(shader_manager.positionNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(unsigned short), vertices_indices, GL_STATIC_DRAW);

			glBindVertexArray(0);
		}


		~Mesh()
		{
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &normals_vbo);
			glDeleteBuffers(1, &ebo);
		};


		void draw(const ShaderManager& shader_manager, const glm::mat4& model, const glm::mat4& view) const {
			glBindVertexArray(vao);
			glUniformMatrix4fv(shader_manager.positionVM, 1, GL_FALSE, glm::value_ptr(view * model));
			/*glUniformMatrix4fv(shader_manager.positionVN, 1, GL_FALSE, 
				glm::value_ptr(view * glm::transpose(glm::inverse(model))));*/
			glUniformMatrix4fv(shader_manager.positionVN, 1, GL_FALSE,
				glm::value_ptr(glm::transpose(glm::inverse(view * model))));
			glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_SHORT, 0);
			glBindVertexArray(0);
		}



		GLuint getVao() const {
			return vao;
		}


	private:
		static long global_mesh_id;

		GLuint vao;
		GLuint vbo;
		GLuint normals_vbo;
		GLuint ebo;

		const long indices_count;

	};



}