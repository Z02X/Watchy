#pragma once

// Epanded
#include "WatchyApp.h"

class CSyncNTP : public CWatchyApp
{
	public:
		CSyncNTP(CWatchyExpanded& expanded);
		const char* Name() const override;
		void        Work()       override;

	private:
		bool SyncNTP(const long gmt, const String& ntpServer);
};

