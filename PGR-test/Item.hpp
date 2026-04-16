#pragma once

#include <glm/glm.hpp>


namespace vasylnaz {

	enum class RenderQueue {
		WINDOW_MASK,
		OPAQUE_INSIDE,
		OPAQUE_OUTSIDE,
		TRANSPARENT_MASK
	};

	class Item {
	public:

		virtual ~Item() = default;

		virtual void updateItem(const glm::mat4& parent_model_matrix) = 0;

	};


}