#pragma once
#include <array>
#include <glm/gtx/rotate_vector.hpp>

#include "Camera.hpp"
#include "pgr.h"
#include "Params.hpp"
#include "Item.hpp"
#include "SceneGraph.hpp"
#include "Object.hpp"

#define KEY_COUNT 256
#define MOUSE_BUTTONS_COUNT 3
#define MAX_DEPTH 1

namespace vasylnaz {
	class InputHandler {
	public:

		InputHandler();


		/// @brief 
		/// @param  
		/// @return 
		const std::array<bool, KEY_COUNT> get_keys_state(void) const {
			return keys_state;
		}

		/// @brief 
		/// @param key_id 
		void set_key_true(const unsigned char& key_id) {
			keys_state[key_id] = true;
		}

		/// @brief 
		/// @param key_id 
		void set_key_false(const unsigned char& key_id) {
			keys_state[key_id] = false;
		}

		/// @brief 
		/// @param key_id 
		void set_special_key_true(const unsigned char& key_id) {
			special_keys_state[key_id] = true;
		}

		/// @brief 
		/// @param key_id 
		void set_special_key_false(const unsigned char& key_id) {
			special_keys_state[key_id] = false;
		}

		/// @brief 
		/// @param button 
		/// @param state 
		/// @param x 
		/// @param y 
		void handle_mouse_click(const int& button, const int& state, const int& x, const int& y);

		/// @brief 
		/// @param x 
		/// @param y 
		void handle_mouse_drag(const int& x, const int& y);

		/// @brief 
		/// @param camera 
		void update_camera_pos(Camera& camera);

		/// @brief 
		/// @param camera 
		void update_camera_target(Camera& camera);

		/// @brief 
		/// @param camera 
		void update_camera(Camera& camera, const ShaderProgram& pick_prog,
			SceneGraph& scene_graph, const glm::mat4& view_mat, const glm::mat4& proj_mat);

		/// @brief 
		/// @param pick_prog 
		/// @param scene_graph 
		/// @param view_mat 
		void pick(const ShaderProgram& pick_prog, SceneGraph& scene_graph,
			const glm::mat4& view_mat, const glm::mat4& proj_mat);


	private:
		struct Mouse {
			std::array<bool, MOUSE_BUTTONS_COUNT> buttons_state;
			glm::vec2 last_coords;
			glm::vec2 new_coords;
		} mouse;

		std::array<bool, KEY_COUNT> keys_state;
		std::array<bool, KEY_COUNT> special_keys_state;
	};

}