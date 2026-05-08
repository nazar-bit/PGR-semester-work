#pragma once

#include "AssetManager.hpp"
#include "NodeScript.hpp"
#include "Request.hpp"
#include "Params.hpp"


namespace vasylnaz {

	class Object;
	class Node;
	class TextObject;
	class NodeScript;
	class FanAnim;


	class Script {
	public:
		Object* owner;

		/// @brief 
		/// @param owner What Object owns the script
		Script(Object* owner) 
			: owner(owner)
		{
			//
		}

		/// @brief 
		virtual ~Script() = default;

		/// @brief Script's main func
		virtual void update() = 0;
	};



	class PCScript : public Script {
	public:

		std::vector<LightSource*> lights;

		/// @brief 
		/// @param owner What Object owns the script
		PCScript(Object* owner);

		/// @brief Script's main func
		void update();

	private:
		bool is_clicked;
		GLuint pc_on_map;
		GLuint pc_off_map;
	};



	class FanSwitch : public Script {
	public:

		FanAnim* fan_anim;

		/// @brief 
		/// @param owner What Object owns the script
		FanSwitch(Object* owner);

		/// @brief Script's main func
		void update();

	private:
	};



	class LightSwitch : public Script {
	public:

		std::vector<Object*> lamps;
		std::vector<LightSource*> lights;
		GLuint lamp_on_map = -1;
		GLuint lamp_off_map = AssetManager::getInstance().getTexture("blank_em");

		/// @brief 
		/// @param owner What Object owns the script
		LightSwitch(Object* owner);

		/// @brief Script's main func
		void update();

	private:
		bool activated;
	};



	class RequestScript : public Script {
	public:

		Request rq;

		/// @brief 
		/// @param owner What Object owns the script
		/// @param rq 
		RequestScript(Object* owner, Request rq);

		/// @brief Script's main func
		void update();

	private:
		bool is_clicked;
	};



	class RequestColorScript : public Script {
	public:

		Request rq;
		Node* text_node;

		/// @brief 
		/// @param owner What Object owns the script
		/// @param rq 
		/// @param text_node 
		RequestColorScript(Object* owner, Request rq, Node* text_node);

		/// @brief Script's main func
		void update();

	private:
		bool is_clicked;
	};
}