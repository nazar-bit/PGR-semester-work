#pragma once

#include <queue>


namespace vasylnaz {

	enum Request {
		NO_REQUEST,
		LAUNCH,
		MAIN_MENU,
		EXIT,
		ACTIVATE_DEBUG,
	};

	extern std::queue<Request> requests;
}