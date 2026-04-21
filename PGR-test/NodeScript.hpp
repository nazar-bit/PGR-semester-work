#pragma once

#include "AssetManager.hpp"
#include "Script.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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
}