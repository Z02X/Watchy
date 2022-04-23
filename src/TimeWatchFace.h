#pragma once

// Expanded
#include "WatchFace.h"
#include "WatchyExpanded.h"

class CTimeWatchFace : public CWatchFace
{
	public:
		virtual void Draw(CWatchyExpanded& expanded) override;

		void StartDraw(CWatchyExpanded::ADisplay& display) const;
		void DrawInternal(CWatchyExpanded& expanded);
};
