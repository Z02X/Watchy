#pragma once

// Expanded
//#include "WatchyExpanded.h" // Here because every user has to include this.

class CWatchyExpanded;

class CWatchFace
{
	public:
		CWatchFace() = default;
		virtual void Draw(CWatchyExpanded& expanded) = 0; //override this method for different watch faces
};
