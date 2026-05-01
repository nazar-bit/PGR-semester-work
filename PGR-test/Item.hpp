#pragma once

#include <glm/glm.hpp>


namespace vasylnaz {

	enum class RenderQueue {
		OPAQUE_MASK,
		LEAF_MASK,
		NO_CULLING_MASK,
		TV_SCREEN_MASK,
		TRANSPARENT_MASK,
		SKYBOX,
		TEXT,
	};


	class Item {
	public:

		virtual ~Item() = default;

		virtual void updateItem(const glm::mat4& parent_model_matrix) = 0;

	};


}