#pragma once

#include "AssetManager.hpp"
#include "Script.hpp"
#include "Curve.hpp"
#include "Utils.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>



namespace vasylnaz {

	class Node;
	class Script;


	class NodeScript {
	public:
		Node* owner;

		NodeScript(Node* owner)
			: owner(owner)
		{

		}

		virtual ~NodeScript() = default;

		virtual void update() = 0;
	};



	class FanAnim : public NodeScript {
	public:

		bool activated = false;

		FanAnim(Node* owner);

		void update();

	private:
		float speed = 0.0f;
		float initial_speed = 0.1f;
		float max_speed = 20.0f;
		float acceleration = 1.02f;
	};



	class CurveMovement : public NodeScript {
	public:

		CurveMovement(Node* owner, std::unique_ptr<Curve> curve);

		void update();

		Curve* getCurve() {
			return curve.get();
		}

	private:
		float speed = 2.0f;
		float movement_start = -1.0f;
		std::unique_ptr<Curve> curve;
	};
}