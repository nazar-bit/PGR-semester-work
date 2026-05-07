#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <array>

#include "pgr.h"
#include "ShaderManager.hpp"
#include "Utils.hpp"



namespace vasylnaz {

	class Curve {
	public:
		std::vector<glm::vec3> points;
		glm::mat4 model = glm::mat4(1.0f);

		/// @brief 
		/// @param input_points Vector of input points
		Curve(const std::vector<glm::vec3>& input_points);

		/// @brief Used for Debug to see the curve
		/// @param number_of_steps Defines how many lines are used to aproximate the curve
		void buildCurve(int number_of_steps);

		/// @brief Takes relative time, gives new position
		/// @param time Relative time
		/// @return glm::vec3 New position
		glm::vec3 moveAlong(float time);

		/// @brief Draws the curve
		/// @param shader_program ShaderProgram&
		/// @param view View matrix
		void draw(const ShaderProgram& shader_program, const glm::mat4& view);

		/// @brief 
		/// @return Last knot 
		float getLastKnot() {
			return knots.back();
		}


	private:
		std::vector<float> knots;
		GLuint vao;
		GLuint vbo;
		unsigned int num_vertices = 0;

		/// @brief Calculates Weights for the curve knots
		/// @param time Relative time
		/// @param knot_ptr The newest knot influencing the curve segment
		/// @return glm::vec4 Weights
		glm::vec4 calcWeighted(float time, unsigned int knot_ptr);
	};
}