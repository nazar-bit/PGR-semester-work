#pragma once

#include "pgr.h"
#include "Material.hpp"
#include "Light.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


namespace vasylnaz {


	enum Attributes {
		POSITION,
		NORMAL,
		TEXCOORD,
		TANGENT,
	};


	class ShaderProgram
	{
	public:
		GLuint shaderProgram = -1;
		GLuint positionLoc = -1;
		GLuint positionNormal = -1;
		GLuint positionTex = -1;
		GLuint positionTangent = -1;

		GLuint positionVM = -1;
		GLuint positionVN = -1;
		GLuint positionP = -1;

		GLuint positionGlobalAmb = -1;
		GLuint positionDiffuseMap = -1;
		GLuint positionNormalMap = -1;
		GLuint positionEmmisionMap = -1;

		GLuint positionId = -1;


		void compile_shaders(const std::string& vert_loc, const std::string& frag_loc);


		void loadMaterial(const Material& material) const;


		void loadDiffuse(const GLuint dif_texture) const;


		void loadNormal(const GLuint normal_texture) const;


		void loadEmission(const GLuint em_texture) const;


		void loadId(const long object_id) const;


		void update_light(const LightBlockData& LBD) const;


		void generateUBOs();


		const GLuint& getUboMaterial() const {
			return uboMaterial;
		}


	private:
		GLuint uboMaterial;
		const GLuint materialBindingPoint = 0;
		const GLuint lightBindingPoint = 1;

		GLuint uboLightBlock;
	};



	std::string loadShaderSource(const std::string& filepath);
}