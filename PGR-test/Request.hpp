#pragma once

#include <queue>


namespace vasylnaz {

	enum Request {
		NO_REQUEST,
		LAUNCH,
		MAIN_MENU,
		EXIT,
	};

	extern std::queue<Request> requests;
}