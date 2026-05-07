#pragma once

#include <queue>


namespace vasylnaz {

	enum Request {
		NO_REQUEST,
		LAUNCH,
		MAIN_MENU,
		EXIT,
		TOGGLE_DEBUG,
		TOGGLE_FOG,
	};

	extern std::queue<Request> requests;
}