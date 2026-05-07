#pragma once
#include <array>
#include <glm/gtx/rotate_vector.hpp>

#include "Camera.hpp"
#include "pgr.h"
#include "Params.hpp"
#include "Item.hpp"
#include "SceneGraph.hpp"
#include "Object.hpp"
#include "Request.hpp"

#define KEY_COUNT 256
#define MOUSE_BUTTONS_COUNT 3
#define MAX_DEPTH 1

namespace vasylnaz {

	enum Scenes {
		MAIN_SCENE,
		MENU,
	};


	class InputHandler {
	public:

		/// @brief 
		InputHandler();


		/// @brief Get the state of all keys
		/// @return KeyStates 
		const std::array<bool, KEY_COUNT> getKeysState(void) const {
			return keys_state;
		}

		/// @brief 
		/// @param key_id Key's id
		void setKeyTrue(const unsigned char& key_id) {
			keys_state[key_id] = true;
		}

		/// @brief 
		/// @param key_id Key's id
		void setKeyFalse(const unsigned char& key_id) {
			keys_state[key_id] = false;
		}

		/// @brief 
		/// @param key_id Key's id
		void setSpecialKeyTrue(const unsigned char& key_id) {
			special_keys_state[key_id] = true;
		}

		/// @brief 
		/// @param key_id Key's id
		void setSpecialKeyFalse(const unsigned char& key_id) {
			special_keys_state[key_id] = false;
		}

		/// @brief 
		/// @param button What mouse button was pressed
		/// @param state Key's state
		/// @param x Position X on the screen
		/// @param y Position Y on the screen
		void handleMouseClick(const int& button, const int& state, const int& x, const int& y);

		/// @brief 
		/// @param x Position X on the screen
		/// @param y Position Y on the screen
		void handleMouseDrag(const int& x, const int& y);

		/// @brief 
		/// @param camera Camera&
		void updateCameraPos(Camera& camera);

		/// @brief 
		/// @param camera Camera&
		void updateCameraTarget(Camera& camera);

		/// @brief Updates camera's position, rotation, does picking, acts on keyboard keys, processes requests
		/// @param camera Camera&
		/// @param pick_prog ShaderProgram& used for picking
		/// @param scene_graph Scene (F.E. MainScene)
		/// @param view_mat View matrix
		/// @param proj_mat Projection matrix
		void update(Camera& camera, const ShaderProgram& pick_prog,
			SceneGraph* scene_graph, const glm::mat4& view_mat, const glm::mat4& proj_mat);

		/// @brief Processes picking
		/// @param pick_prog ShaderProgram& used for picking
		/// @param scene_graph Scene (F.E. MainScene)
		/// @param view_mat View matrix
		void pick(const ShaderProgram& pick_prog, SceneGraph* scene_graph,
			const glm::mat4& view_mat, const glm::mat4& proj_mat);

		/// @brief 
		/// @param scene unique ptr to SceneGraph
		void addScene(std::unique_ptr<SceneGraph> scene);

		/// @brief 
		/// @param camera Camera&
		void loadMainScene(Camera& camera);

		/// @brief 
		/// @param camera Camera&
		void loadMenu(Camera& camera);

		/// @brief 
		/// @return Menu's scene graph
		const SceneGraph* getMenu() {
			return scenes[MENU].get();
		}


	private:
		struct Mouse {
			std::array<bool, MOUSE_BUTTONS_COUNT> buttons_state;
			glm::vec2 last_coords;
			glm::vec2 new_coords;
		} mouse;

		std::array<bool, KEY_COUNT> keys_state;
		std::array<bool, KEY_COUNT> special_keys_state;

		std::vector<std::unique_ptr<SceneGraph>> scenes;

		bool camera_locked = true;

		/// @brief 
		/// @param camera Camera&
		void checkKeysPressed(Camera& camera);

		/// @brief Processes requests
		/// @param camera Camera&
		void checkRequests(Camera& camera);
	};

}