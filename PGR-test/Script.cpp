#include "Script.hpp"
#include "Object.hpp"


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
		: Script(owner)
	{
		//
	}


	void LightSwitch::update() {
		if (activated != owner->clicked) {
			activated = owner->clicked;
			for (auto& lamp : lamps) {
				if (activated) {
					lamp->em_map = lamp_on_map;
				}
				else {
					lamp->em_map = lamp_off_map;
				}
			}
		}
	}


}

