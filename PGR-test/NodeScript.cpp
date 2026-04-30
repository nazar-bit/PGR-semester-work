#include "NodeScript.hpp"
#include "SceneGraph.hpp"
#include "Object.hpp"


namespace vasylnaz {


	FanAnim::FanAnim(Node* owner)
		: NodeScript(owner)
	{
		//
	}


	void FanAnim::update() {
		if (activated) {
			if (speed < initial_speed) {
				speed = initial_speed;
			}
			else if (speed > max_speed) {
				speed = max_speed;
			}
			speed *= acceleration;
		}
		else {
			if (speed < initial_speed) {
				speed = 0.0f;
			}
			speed /= acceleration;
		}

		owner->model_mat = glm::rotate(owner->model_mat, glm::radians(speed), glm::vec3(0.0f, 1.0f, 0.0f));
	}




	CurveMovement::CurveMovement(Node* owner, std::unique_ptr<Curve> curve)
		: NodeScript(owner), curve(std::move(curve))
	{
		//
	}


	void CurveMovement::update() {

		auto new_pos = curve->moveAlong((GlobalTime - movement_start) * speed);
		if (glm::any(glm::isnan(new_pos))) {
			movement_start = GlobalTime;
			return;
		}
		else {
			glm::vec3 scale(
				glm::length(glm::vec3(owner->model_mat[0])),
				glm::length(glm::vec3(owner->model_mat[1])),
				glm::length(glm::vec3(owner->model_mat[2]))
			);

			glm::quat new_rotation = calculateRotation(owner->model_mat[3], new_pos);
			glm::quat adjustment = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			new_rotation = glm::normalize(adjustment * new_rotation); 

			glm::mat4 rebuilt_mat = glm::translate(glm::mat4(1.0f), new_pos);
			rebuilt_mat = rebuilt_mat * glm::mat4_cast(new_rotation);
			rebuilt_mat = glm::scale(rebuilt_mat, scale);

			owner->model_mat = rebuilt_mat;
			//owner->model_mat[3] = glm::vec4(new_pos, 1.0f);
		}
	}





}