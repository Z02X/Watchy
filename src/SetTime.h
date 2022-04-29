#pragma once

// Epanded
#include "WatchyApp.h"

class CSetTime : public CWatchyApp
{
	public:
		CSetTime();
		       const char* Name() const override;
		       void        Work(CWatchyExpanded& expanded) override;
};
