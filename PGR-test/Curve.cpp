#include "Curve.hpp"


namespace vasylnaz {

	glm::vec3 debugColor = glm::vec3(1.0f, 0.0f, 0.0f);


	Curve::Curve(const std::vector<glm::vec3>& input_points)
		: points(input_points)
	{
		if (input_points.size() < 4) {
			throw std::invalid_argument("Curve must have at least 4 points");
		}

		std::vector<float> euc_distances;
		for (int i = 0; i < points.size() - 1; ++i) {
			euc_distances.push_back(glm::distance(points[i], points[i + 1]));
		}

		float alpha = 0.5f;
		std::vector<float> raw_knots;
		raw_knots.push_back(0.0f);

		for (int i = 0; i < euc_distances.size(); ++i) {
			raw_knots.push_back(raw_knots[i] + glm::pow(euc_distances[i], alpha));
		}
		// First 4
		for (int i = 0; i < 4; ++i) {
			knots.push_back(0.0f);
		}
		// Inner
		for (int i = 2; i < points.size() - 2; ++i) {
			knots.push_back(raw_knots[i]);
		}
		// Last 4
		for (int i = 0; i < 4; ++i) {
			knots.push_back(raw_knots.back());
		}
	}



	void Curve::buildCurve(int number_of_steps) {

		std::vector<glm::vec3> path;
		float time_step = knots.back() / number_of_steps;
		unsigned int knot_ptr = 3;

		for (int step = 0; step <= number_of_steps; ++step) {

			float time = (static_cast<float>(step) / number_of_steps) * knots.back();
			while (time >= knots[knot_ptr + 1] && knot_ptr < points.size() - 1) {
				knot_ptr++;
			}

			glm::vec4 weighted = calcWeighted(time, knot_ptr);
			glm::mat4x3 points_mat = glm::mat4x3(
				points[knot_ptr - 3],
				points[knot_ptr - 2],
				points[knot_ptr - 1],
				points[knot_ptr]
			);
			path.push_back(points_mat * weighted);
		}


		num_vertices = path.size();


		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, path.size() * sizeof(glm::vec3), path.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(Attributes::POSITION);
		glVertexAttribPointer(Attributes::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}





	glm::vec3 Curve::moveAlong(float time) {
		unsigned int knot_ptr = 3;

		while (time >= knots[knot_ptr + 1]) {
			if (knot_ptr < points.size() - 1) {
				knot_ptr++;
			}
			else {
				return glm::vec3(NAN_FL, NAN_FL, NAN_FL);
			}
		}

		glm::vec4 weighted = calcWeighted(time, knot_ptr);
		glm::mat4x3 points_mat = glm::mat4x3(
			points[knot_ptr - 3],
			points[knot_ptr - 2],
			points[knot_ptr - 1],
			points[knot_ptr]
		);
		return points_mat * weighted;
	}



	void Curve::draw(const ShaderProgram& shader_manager, const glm::mat4& view) {
		glBindVertexArray(vao);
		glUniformMatrix4fv(shader_manager.positionVM, 1, GL_FALSE, glm::value_ptr(view * model));

		glDrawArrays(GL_LINE_STRIP, 0, num_vertices);
		glBindVertexArray(0);
	}



	glm::vec4 Curve::calcWeighted(float time, unsigned int knot_ptr) {
		glm::vec4 res;
		const unsigned int weighted_size = 7;
		std::array<float, weighted_size> weighted;
		for (int level = 0; level < 4; ++level) {
			for (int i = 0; i < weighted_size - level; ++i) {
				float current_knot = knots[knot_ptr + i - 3];
				float next_knot = knots[knot_ptr + i - 2];
				if (level == 0) {
					bool is_last_knot = (next_knot == knots.back());
					if (time >= current_knot && (time < next_knot || (is_last_knot && time == next_knot))) {
						weighted[i] = 1.0f;
					}
					else {
						weighted[i] = 0.0f;
					}
				}
				else {
					float denominator = knots[knot_ptr + i - 3 + level] - current_knot;
					float A = 0;
					if (denominator != 0) {
						A = ((time - current_knot) / denominator) * weighted[i];
					}
					denominator = knots[knot_ptr + i - 3 + level + 1] - next_knot;
					float B = 0;
					if (denominator != 0) {
						B = ((knots[knot_ptr + i - 3 + level + 1] - time) / denominator) * weighted[i + 1];
					}
					weighted[i] = A + B;
				}
			}
		}


		res.x = weighted[0];
		res.y = weighted[1];
		res.z = weighted[2];
		res.w = weighted[3];

		return res;
	}
}