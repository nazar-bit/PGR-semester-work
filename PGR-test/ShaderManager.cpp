#include "ShaderManager.hpp"


namespace vasylnaz {


	
	void ShaderProgram::generateUBOs() {
		// ------- Material
		glGenBuffers(1, &uboMaterial);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMaterial);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Material), NULL, GL_DYNAMIC_DRAW);

		glBindBufferBase(GL_UNIFORM_BUFFER, materialBindingPoint, uboMaterial);
		GLuint blockIndex = glGetUniformBlockIndex(shaderProgram, "Material");

		glUniformBlockBinding(shaderProgram, blockIndex, materialBindingPoint);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// ------- Light
		glGenBuffers(1, &uboLightBlock);
		glBindBuffer(GL_UNIFORM_BUFFER, uboLightBlock);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(LightBlockData), NULL, GL_DYNAMIC_DRAW);

		glBindBufferBase(GL_UNIFORM_BUFFER, lightBindingPoint, uboLightBlock);
		blockIndex = glGetUniformBlockIndex(shaderProgram, "LightBlock");

		glUniformBlockBinding(shaderProgram, blockIndex, lightBindingPoint);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}


	void ShaderProgram::loadMaterial(const Material& material) const {
		glBindBuffer(GL_UNIFORM_BUFFER, uboMaterial);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Material), &material);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}


	void ShaderProgram::loadDiffuse(const GLuint dif_texture) const {
		if (positionDiffuseMap != -1) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, dif_texture);
			glUniform1i(positionDiffuseMap, 0);
		}
	}


	void ShaderProgram::loadNormal(const GLuint normal_texture) const {
		if (positionNormalMap != -1) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normal_texture);
			glUniform1i(positionNormalMap, 1);
		}
	}


	void ShaderProgram::loadEmission(const GLuint em_texture) const {
		if (positionEmmisionMap != -1) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, em_texture);
			glUniform1i(positionEmmisionMap, 2);
		}
	}


	void ShaderProgram::update_light(const LightBlockData& LBD) const {
		glBindBuffer(GL_UNIFORM_BUFFER, uboLightBlock);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightBlockData), &LBD);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}


	void ShaderProgram::loadId(const long object_id) const {
		if (positionId != -1)
		{
			glUniform1f(positionId, (float)object_id / 255.0f);
		}
	}



	void ShaderProgram::compile_shaders(const std::string& vert_loc, const std::string& frag_loc) {
		std::string vertexShaderSrc = loadShaderSource(vert_loc);
		std::string fragmentShaderSrc = loadShaderSource(frag_loc);

		GLuint shaders[] = {
			pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
			pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc),
			0
		};
		shaderProgram = pgr::createProgram(shaders);
		positionLoc = glGetAttribLocation(shaderProgram, "position");
		positionNormal = glGetAttribLocation(shaderProgram, "normal");
		positionTex = glGetAttribLocation(shaderProgram, "tex_coords");
		positionTangent = glGetAttribLocation(shaderProgram, "tangent");

		positionVM = glGetUniformLocation(shaderProgram, "VM");
		positionVN = glGetUniformLocation(shaderProgram, "VN");
		positionP = glGetUniformLocation(shaderProgram, "P");

		positionGlobalAmb = glGetUniformLocation(shaderProgram, "global_ambient");
		positionDiffuseMap = glGetUniformLocation(shaderProgram, "texSampler");
		positionNormalMap = glGetUniformLocation(shaderProgram, "normSampler");
		positionEmmisionMap = glGetUniformLocation(shaderProgram, "emSampler");

		positionId = glGetUniformLocation(shaderProgram, "id");
	}



	std::string loadShaderSource(const std::string& filepath) {
		std::ifstream file(filepath);

		if (!file.is_open()) {
			std::cerr << "ERROR: Failed to open shader file: " << filepath << std::endl;
			return "";
		}

		std::stringstream stream;
		stream << file.rdbuf();
		file.close();

		return stream.str();
	}

}