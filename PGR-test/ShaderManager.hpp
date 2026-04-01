#pragma once

#include "pgr.h"
#include "Material.hpp"
#include "Light.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


namespace vasylnaz {

	class ShaderManager
	{
	public:
		GLuint shaderProgram = 0;
		GLuint positionLoc = 0;
		GLuint positionNormal = 0;

		GLuint positionVM = 0;
		GLuint positionVN = 0;
		GLuint positionP = 0;

		GLuint positionGlobalAmb = 0;


		void compile_shaders() {
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

			positionVM = glGetUniformLocation(shaderProgram, "VM");
			positionVN = glGetUniformLocation(shaderProgram, "VN");
			positionP = glGetUniformLocation(shaderProgram, "P");

			positionGlobalAmb = glGetUniformLocation(shaderProgram, "global_ambient");
		}


		void generateUBOs() {
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


		const GLuint& getUboMaterial() const {
			return uboMaterial;
		}


		void change_material(const Material& material) const {
			glBindBuffer(GL_UNIFORM_BUFFER, uboMaterial);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Material), &material);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}


		void update_light(const LightBlockData& LBD) const {
			glBindBuffer(GL_UNIFORM_BUFFER, uboLightBlock);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightBlockData), &LBD);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}



	private:
		const GLuint materialBindingPoint = 0;
		const GLuint lightBindingPoint = 1;

		GLuint uboMaterial;
		GLuint uboLightBlock;


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
	};



}