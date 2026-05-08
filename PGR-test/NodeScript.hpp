#pragma once

#include "AssetManager.hpp"
#include "Script.hpp"
#include "Curve.hpp"
#include "Utils.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <cmath>



namespace vasylnaz {

	class Node;
	class Script;
	class Object;

	/// @brief 
	class NodeScript {
	public:
		Node* owner;

		/// @brief 
		/// @param owner What node owns the script
		NodeScript(Node* owner)
			: owner(owner)
		{

		}

		/// @brief 
		virtual ~NodeScript() = default;

		/// @brief Script's main code, called every frame
		virtual void update() = 0;
	};



	class FanAnim : public NodeScript {
	public:

		bool activated = false;

		/// @brief 
		/// @param owner What node owns the script
		FanAnim(Node* owner);

		/// @brief Script's main code, called every frame
		void update();

	private:
		float speed = 0.0f;
		float initial_speed = 0.1f;
		float max_speed = 20.0f;
		float acceleration = 1.02f;
	};



	class CurveMovement : public NodeScript {
	public:

		Object* wheels;

		/// @brief 
		/// @param owner What node owns the script
		/// @param curve Unique ptr to curve, the script is following
		CurveMovement(Node* owner, std::unique_ptr<Curve> curve);

		/// @brief Script's main code, called every frame
		void update();

		/// @brief 
		/// @return 
		Curve* getCurve() {
			return curve.get();
		}

	private:
		float speed = 2.0f;
		float rotation_speed = speed * 15.0f;
		std::unique_ptr<Curve> curve;
	};
}