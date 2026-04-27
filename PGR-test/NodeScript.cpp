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
		float global_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

		auto new_pos = curve->moveAlong((global_time - movement_start) * speed);
		if (glm::any(glm::isnan(new_pos))) {
			movement_start = global_time;
			return;
		}
		else {
			owner->model_mat[3] = glm::vec4(new_pos, 1.0f);
		}
	}





}