#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <array>

#include "pgr.h"
#include "ShaderManager.hpp"
#include "Utils.hpp"



namespace vasylnaz {

	extern glm::vec3 debugColor;

	class Curve {
	public:
		std::vector<glm::vec3> points;
		glm::mat4 model = glm::mat4(1.0f);


		Curve(const std::vector<glm::vec3>& input_points);

		void buildCurve(int number_of_steps);

		glm::vec3 moveAlong(float time);

		void draw(const ShaderProgram& shader_manager, const glm::mat4& view);

		float getLastKnot() {
			return knots.back();
		}


	private:
		std::vector<float> knots;
		GLuint vao;
		GLuint vbo;
		unsigned int num_vertices = 0;

		glm::vec4 calcWeighted(float time, unsigned int knot_ptr);
	};
}