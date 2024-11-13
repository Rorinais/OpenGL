#pragma once
#include "../global/base.h"

class Identity {
public:
	static ID generateID() { return ++mCurrentID; }

private:
	static ID mCurrentID;
};
	
