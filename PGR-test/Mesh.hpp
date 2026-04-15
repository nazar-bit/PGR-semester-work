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
		/// @param vertices_pos 
		/// @param vertices_count 
		/// @param normals 
		/// @param vertices_indices 
		/// @param indices_count 
		/// @param shader_manager 
		Mesh(const float* vertices_pos, const long vertices_count, const float* normals,
			const unsigned int* vertices_indices, const long indices_count,
			ShaderManager& shader_manager);


		/// @brief 
		/// @param filePath 
		/// @param shader_manager 
		Mesh(const string& filePath, ShaderManager& shader_manager);

		/// @brief 
		/// @param mesh 
		/// @param shader_manager 
		Mesh(const aiMesh* mesh, ShaderManager& shader_manager);


		void setBuffers(const aiMesh* mesh, ShaderManager& shader_manager);


		~Mesh();


		/// @brief 
		/// @param shader_manager 
		/// @param model 
		/// @param view 
		void draw(const ShaderManager& shader_manager, const glm::mat4& model, const glm::mat4& view) const;



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

	};
}