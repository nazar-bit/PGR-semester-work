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
		GLuint positionTex = 0;
		GLuint positionTangent = 0;

		GLuint positionVM = 0;
		GLuint positionVN = 0;
		GLuint positionP = 0;

		GLuint positionGlobalAmb = 0;
		GLuint positionDiffuseMap = 0;
		GLuint positionNormalMap = 0;


		void compile_shaders();


		void generateUBOs();


		const GLuint& getUboMaterial() const {
			return uboMaterial;
		}


		void change_material(const Material& material) const;


		void update_light(const LightBlockData& LBD) const;


	private:
		const GLuint materialBindingPoint = 0;
		const GLuint lightBindingPoint = 1;

		GLuint uboMaterial;
		GLuint uboLightBlock;


		std::string loadShaderSource(const std::string& filepath);
	};



}