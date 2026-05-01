#pragma once

#include <queue>


namespace vasylnaz {

	enum Request {
		NO_REQUEST,
		LAUNCH,
		MAIN_MENU
	};

	extern std::queue<Request> requests;
}