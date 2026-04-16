#include "ShaderManager.hpp"


namespace vasylnaz {


	void ShaderManager::compile_shaders() {
		std::string vertexShaderSrc = loadShaderSource("basic.vert");
		std::string fragmentShaderSrc = loadShaderSource("basic.frag");

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
	}


	void ShaderManager::generateUBOs() {
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


	void ShaderManager::change_material(const Material& material) const {
		glBindBuffer(GL_UNIFORM_BUFFER, uboMaterial);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Material), &material);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}


	void ShaderManager::update_light(const LightBlockData& LBD) const {
		glBindBuffer(GL_UNIFORM_BUFFER, uboLightBlock);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightBlockData), &LBD);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}



	std::string ShaderManager::loadShaderSource(const std::string& filepath) {
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