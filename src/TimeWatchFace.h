#pragma once

// Time
#include <time.h>

// Expanded
#include "WatchFace.h"
#include "WatchyExpanded.h"

class CTimeWatchFace : public CWatchFace
{
	public:
		virtual void Draw(CWatchyExpanded& expanded) override;

		void StartDraw(CWatchyExpanded::ADisplay& display);
		void DrawInternal(CWatchyExpanded& expanded);

	protected:
		tm m_currentLocalTime;
};
