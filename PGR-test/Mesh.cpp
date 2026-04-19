#include "Mesh.hpp"


namespace vasylnaz {

	long Mesh::global_mesh_id = 0;

	Mesh::Mesh(const float* vertices_pos, const long vertices_count, const float* normals,
		const unsigned int* vertices_indices, const long indices_count,
		ShaderProgram& shader_manager)

		: mesh_id(global_mesh_id++), indices_count(indices_count) {

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(float), vertices_pos, GL_STATIC_DRAW);
		glEnableVertexAttribArray(Attributes::POSITION);
		glVertexAttribPointer(Attributes::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &normals_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(float), normals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(Attributes::NORMAL);
		glVertexAttribPointer(Attributes::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(unsigned int), vertices_indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}



	Mesh::Mesh(const string& filePath, ShaderProgram& shader_manager)

		: mesh_id(global_mesh_id++), indices_count(0) {

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filePath,
			aiProcess_Triangulate |           // Transform all primitives to triangles
			aiProcess_JoinIdenticalVertices | // Merge identical vertices
			aiProcess_CalcTangentSpace        // Calculate tangents and bitangents
		);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "ASSIMP Error: " << importer.GetErrorString() << std::endl;
			return;
		}
		std::cout << "Successfully loaded: " << filePath << std::endl;
		std::cout << "Total meshes found: " << scene->mNumMeshes << std::endl;

		aiMesh* mesh = scene->mMeshes[0];

		setBuffers(mesh, shader_manager);
	}



	Mesh::Mesh(const aiMesh* mesh, ShaderProgram& shader_manager)
		: mesh_id(global_mesh_id++), indices_count(0) {

		setBuffers(mesh, shader_manager);
	}



	void Mesh::setBuffers(const aiMesh* mesh, ShaderProgram& shader_program) {
		std::vector<unsigned int> indices;
		indices.reserve(mesh->mNumFaces * 3);

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}
		indices_count = indices.size();

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(Attributes::POSITION);
		glVertexAttribPointer(Attributes::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &normals_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mNormals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(Attributes::NORMAL);
		glVertexAttribPointer(Attributes::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);

		if (mesh->HasTextureCoords(0)) {
			glGenBuffers(1, &texels_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, texels_vbo);
			glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mTextureCoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(Attributes::TEXCOORD);
			glVertexAttribPointer(Attributes::TEXCOORD, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else {
			glDisableVertexAttribArray(Attributes::TEXCOORD);
		}

		if (mesh->HasTangentsAndBitangents()) {
			glGenBuffers(1, &tangents_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, tangents_vbo);
			glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mTangents, GL_STATIC_DRAW);
			glEnableVertexAttribArray(Attributes::TANGENT);
			glVertexAttribPointer(Attributes::TANGENT, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else {
			glDisableVertexAttribArray(Attributes::TANGENT);
			glVertexAttrib3f(Attributes::TANGENT, 1.0f, 0.0f, 0.0f);
		}

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}



	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &normals_vbo);
		glDeleteBuffers(1, &texels_vbo);
		glDeleteBuffers(1, &tangents_vbo);
		glDeleteBuffers(1, &ebo);
	};


	void Mesh::draw(const ShaderProgram& shader_manager, const glm::mat4& model, const glm::mat4& view) const {
		glBindVertexArray(vao);

		glUniformMatrix4fv(shader_manager.positionVM, 1, GL_FALSE, glm::value_ptr(view * model));
		if (shader_manager.positionVN != -1)
		{
			glUniformMatrix4fv(shader_manager.positionVN, 1, GL_FALSE,
			glm::value_ptr(glm::transpose(glm::inverse(view * model))));
		}

		glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}