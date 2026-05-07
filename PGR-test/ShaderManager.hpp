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


	const int SHADOW_WIDTH = 1024;
	const int SHADOW_HEIGHT = 1024;


	const extern GLuint MATERIAL_BINDING_POINT;
	const extern GLuint LIGHT_BINDING_POINT;

	extern GLuint globalUboMaterial;
	extern GLuint globalUboLight;

	extern GLuint depthTextureArray;
	extern GLuint shadowMapFBO;

	void initializeSharedUBOs();

	void initializeShadowFBO();

	void updateLights(const LightBlockData& newLightData);



	class ShaderProgram
	{
	public:
		GLuint shaderProgram = -1;
		GLuint positionLoc = -1;
		GLuint positionNormal = -1;
		GLuint positionTex = -1;
		GLuint positionTangent = -1;

		GLuint positionM = -1;
		GLuint positionV = -1;
		GLuint positionP = -1;
		GLuint positionTime = -1;

		GLuint positionGlobalAmb = -1;
		GLuint positionDiffuseMap = -1;
		GLuint positionNormalMap = -1;
		GLuint positionEmmisionMap = -1;
		GLuint positionShadowMap = -1;

		GLuint positionId = -1;
		GLuint positionColor = -1;
		GLuint positionFog = -1;
		GLuint positionFogDensity = -1;

		GLuint positionLightSpaceM = -1;


		void compile_shaders(const std::string& vert_loc, const std::string& frag_loc);


		void loadMaterial(const Material& material) const;


		void loadDiffuse(const GLuint dif_texture) const;


		void loadNormal(const GLuint normal_texture) const;


		void loadEmission(const GLuint em_texture) const;


		void loadShadowMap(const GLuint map) const;


		void loadId(const long object_id) const;


		void loadTextColor(glm::vec3 text_color) const;


		void bindUBOs();




	private:
	};



	std::string loadShaderSource(const std::string& filepath);




}