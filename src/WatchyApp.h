#pragma once

// Expanded
#include "WatchyExpanded.h"

class CWatchyApp
{
	public:
		CWatchyApp();
		virtual const char* Name() const                    = 0;
		virtual void        Work(CWatchyExpanded& expanded) = 0;
};
