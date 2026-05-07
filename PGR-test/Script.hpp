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
		/// @param owner 
		Script(Object* owner) 
			: owner(owner)
		{
			//
		}

		virtual ~Script() = default;

		virtual void update() = 0;
	};



	class PCScript : public Script {
	public:

		std::vector<LightSource*> lights;

		/// @brief 
		/// @param owner 
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

		/// @brief 
		/// @param owner 
		FanSwitch(Object* owner);

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
		/// @param owner 
		LightSwitch(Object* owner);

		void update();

	private:
		bool activated;
	};



	class RequestScript : public Script {
	public:

		Request rq;

		/// @brief 
		/// @param owner 
		/// @param rq 
		RequestScript(Object* owner, Request rq);

		void update();

	private:
		bool is_clicked;
	};



	class RequestColorScript : public Script {
	public:

		Request rq;
		Node* text_node;

		/// @brief 
		/// @param owner 
		/// @param rq 
		/// @param text_node 
		RequestColorScript(Object* owner, Request rq, Node* text_node);

		void update();

	private:
		bool is_clicked;
	};
}