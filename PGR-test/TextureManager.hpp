#pragma once

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

#include "pgr.h"

using std::string;


namespace vasylnaz {
	class TextureManager {
	public:

		GLint diffuse_map_location;


		TextureManager()
		{
			//
		}

		
		void addTetxure(const string& name, const string& file_path) {
			GLuint texID = pgr::createTexture(file_path);
			textures.emplace(name, texID);
		}


		void feed_diffuse_map(const string& texture_name) const {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures.at(texture_name));
			glUniform1i(diffuse_map_location, 0);
		}




	private:
		std::unordered_map<string, GLuint> textures;

	};





}