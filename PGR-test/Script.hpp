#pragma once

#include "AssetManager.hpp"
#include "NodeScript.hpp"


namespace vasylnaz {

	class Object;
	class NodeScript;
	class FanAnim;


	class Script {
	public:
		Object* owner;

		Script(Object* owner) 
			: owner(owner)
		{

		}

		virtual ~Script() = default;

		virtual void update() = 0;
	};



	class PCScript : public Script {
	public:

		PCScript(Object* owner);

		void update();

	private:
		bool is_clicked;
		GLuint pc_on_map;
		GLuint pc_off_map;
	};



	class FanSwitch : public Script {
	public:

		FanAnim* fan_anim;

		FanSwitch(Object* owner);

		void update();

	private:
	};
}