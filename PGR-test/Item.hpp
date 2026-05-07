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
		COUNT // MUST BE LAST
	};


	class Item {
	public:

		virtual ~Item() = default;

		/// @brief Updates item in SceneGraph
		/// @param parent_model_matrix Parent's model matrix
		virtual void updateItem(const glm::mat4& parent_model_matrix) = 0;
	};
}