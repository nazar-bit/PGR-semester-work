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

	/// @brief Initialize UBOs used for lighting and material
	void initializeSharedUBOs();

	/// @brief Initialize FBO used for shadow mapping
	void initializeShadowFBO();

	/// @brief Transfer updated LightData to the shaders
	/// @param newLightData Updated LightBlockData
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


		/// @brief 
		/// @param vert_loc Location of the vertex shader
		/// @param frag_loc Location of the fragment shader
		void compileShaders(const std::string& vert_loc, const std::string& frag_loc);

		/// @brief 
		/// @param material Material&
		void loadMaterial(const Material& material) const;

		/// @brief 
		/// @param dif_texture Diffuse texture
		void loadDiffuse(const GLuint dif_texture) const;

		/// @brief 
		/// @param normal_texture Normal map
		void loadNormal(const GLuint normal_texture) const;

		/// @brief 
		/// @param em_texture Emission map
		void loadEmission(const GLuint em_texture) const;

		/// @brief 
		/// @param map Shadow map
		void loadShadowMap(const GLuint map) const;

		/// @brief 
		/// @param object_id Object's id
		void loadId(const long object_id) const;

		/// @brief 
		/// @param text_color Text color
		void loadTextColor(glm::vec3 text_color) const;

		/// @brief Bind shared UBOs to this shader
		void bindUBOs();


	private:
	};


	/// @brief Loads shader's source code
	/// @param filepath FilePath to the shader code
	/// @return Source code
	std::string loadShaderSource(const std::string& filepath);
}