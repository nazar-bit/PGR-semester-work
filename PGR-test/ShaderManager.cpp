#include "ShaderManager.hpp"


namespace vasylnaz {


	const GLuint MATERIAL_BINDING_POINT = 0;
	const GLuint LIGHT_BINDING_POINT = 1;

	GLuint globalUboMaterial = 0;
	GLuint globalUboLight = 0;


	void initializeSharedUBOs() {
		glGenBuffers(1, &globalUboMaterial);
		glBindBuffer(GL_UNIFORM_BUFFER, globalUboMaterial);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Material), NULL, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, MATERIAL_BINDING_POINT, globalUboMaterial);

		glGenBuffers(1, &globalUboLight);
		glBindBuffer(GL_UNIFORM_BUFFER, globalUboLight);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(LightBlockData), NULL, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_BINDING_POINT, globalUboLight);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}



	void updateLights(const LightBlockData& newLightData) {
		glBindBuffer(GL_UNIFORM_BUFFER, globalUboLight);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightBlockData), &newLightData);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}


	
	void ShaderProgram::bindUBOs() {
		// Material
		GLuint materialBlockIndex = glGetUniformBlockIndex(shaderProgram, "Material");
		if (materialBlockIndex != GL_INVALID_INDEX) {
			glUniformBlockBinding(shaderProgram, materialBlockIndex, MATERIAL_BINDING_POINT);
		}

		// Light
		GLuint lightBlockIndex = glGetUniformBlockIndex(shaderProgram, "LightBlock");
		if (lightBlockIndex != GL_INVALID_INDEX) {
			glUniformBlockBinding(shaderProgram, lightBlockIndex, LIGHT_BINDING_POINT);
		}
	}


	void ShaderProgram::loadMaterial(const Material& material) const {
		glBindBuffer(GL_UNIFORM_BUFFER, globalUboMaterial);
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


	void ShaderProgram::loadId(const long object_id) const {
		if (positionId != -1)
		{
			glUniform1i(positionId, (int)object_id);
		}
	}


	void ShaderProgram::loadTextColor(glm::vec3 text_color) const {
		if (positionColor != -1)
		{
			glUniform3fv(positionColor, 1, glm::value_ptr(text_color));
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
		positionTime = glGetUniformLocation(shaderProgram, "time");

		positionGlobalAmb = glGetUniformLocation(shaderProgram, "global_ambient");
		positionDiffuseMap = glGetUniformLocation(shaderProgram, "texSampler");
		positionNormalMap = glGetUniformLocation(shaderProgram, "normSampler");
		positionEmmisionMap = glGetUniformLocation(shaderProgram, "emSampler");

		positionId = glGetUniformLocation(shaderProgram, "id");
		positionColor = glGetUniformLocation(shaderProgram, "color_fg");
		if (positionColor == -1) {
			positionColor = glGetUniformLocation(shaderProgram, "textColor");
		}
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