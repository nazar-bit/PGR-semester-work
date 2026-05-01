#pragma once

#include "SceneGraph.hpp"


namespace vasylnaz {

	class Menu : public SceneGraph {
	public:

		Menu()
			: SceneGraph()
		{
			//
		}
		 
		void init(ShaderProgram& shader_manager, Camera& camera);

	private:
	};
}