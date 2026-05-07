#include "Script.hpp"
#include "Object.hpp"
#include "SceneGraph.hpp"


namespace vasylnaz {


	PCScript::PCScript(Object* owner)
		: Script(owner), pc_on_map(owner->em_map), 
		pc_off_map(AssetManager::getInstance().getTexture("blank_em"))
	{
		//
	}



	void PCScript::update() {
		if (is_clicked != owner->clicked) {
			if (owner->em_map == pc_off_map) {
				owner->em_map = pc_on_map;
			}
			else {
				owner->em_map = pc_off_map;
			}

			is_clicked = owner->clicked;
		}
	}




	FanSwitch::FanSwitch(Object* owner)
		: Script(owner)
	{
		//
	}


	void FanSwitch::update() {
		fan_anim->activated = owner->clicked;
	}



	LightSwitch::LightSwitch(Object* owner)
		: Script(owner), activated(owner->clicked)
	{
		//
	}


	void LightSwitch::update() {
		if (activated != owner->clicked) {
			activated = owner->clicked;
			for (auto& lamp : lamps) {
				if (activated) {
					lamp->em_map = lamp_off_map;
				}
				else {
					lamp->em_map = lamp_on_map;
				}
			}
			for (auto& light : lights) {
				if (activated) {
					light->deactivateLight();
				}
				else {
					light->activateLight();
				}
			}
		}
	}




	RequestScript::RequestScript(Object* owner, Request rq)
		: Script(owner), is_clicked(owner->clicked), rq(rq)
	{
		//
	}


	void RequestScript::update() {
		if (is_clicked != owner->clicked) {
			requests.push(rq);
			is_clicked = owner->clicked;
		}
	}




	RequestColorScript::RequestColorScript(Object* owner, Request rq, Node* text_node)
		: Script(owner), is_clicked(owner->clicked), rq(rq), text_node(text_node)
	{
		//
	}


	void RequestColorScript::update() {
		if (is_clicked != owner->clicked) {
			requests.push(rq);
			is_clicked = owner->clicked;
			for (auto& item : text_node->items) {
				auto obj = static_cast<TextObject*>(item.get());
				if (owner->clicked) {
					obj->text_color = MENU_ACTIVATED_TEXT_COLOR;
				}
				else {
					obj->text_color = MENU_TEXT_COLOR;
				}
			}
		}
	}
}

