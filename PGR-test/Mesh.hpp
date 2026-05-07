#pragma once

#include "pgr.h"
#include "ShaderManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include <assimp/Importer.hpp>     
#include <assimp/scene.h>           
#include <assimp/postprocess.h>    

#include <string>

using std::string;


namespace vasylnaz {
	class Mesh
	{
	public:
		const long mesh_id;

		/// @brief 
		/// @param vertices_pos Array of vertices positions
		/// @param vertices_count Number of vertices
		/// @param texture_coords Array of texture coordinates
		/// @param vertices_indices Array of vertices indices
		/// @param indices_count Number of indices
		Mesh(const float* vertices_pos, const long vertices_count, const float* texture_coords,
			const unsigned int* vertices_indices, const long indices_count);


		/// @brief 
		/// @param filePath FilePath to the Mesh .obj file
		Mesh(const string& filePath);

		/// @brief 
		/// @param mesh aiMesh* from Assimp lib
		Mesh(const aiMesh* mesh);

		/// @brief 
		~Mesh();

		/// @brief 
		/// @param shader_program ShaderProgram&
		/// @param model Model matrix
		/// @param view View matrix
		void draw(const ShaderProgram& shader_program, const glm::mat4& model, const glm::mat4& view) const;

		/// @brief 
		/// @return VAO
		GLuint getVao() const {
			return vao;
		}


	private:
		static long global_mesh_id;

		GLuint vao;
		GLuint vbo;
		GLuint normals_vbo;
		GLuint texels_vbo;
		GLuint tangents_vbo;
		GLuint ebo;

		long indices_count;

		/// @brief Sets meshe's VAO, VBO, etc ...
		/// @param mesh 
		void setBuffers(const aiMesh* mesh);
	};
}