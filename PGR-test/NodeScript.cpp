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





}