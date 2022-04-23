// Self
#include "DateWatchFace.h"

// Fonts
#include <Fonts/FreeMonoBold9pt7b.h>

// Time
#include <TimeLib.h>

void CDateWatchFace::Draw(CWatchyExpanded& expanded)
{
	StartDraw(expanded.Display());
	CTimeWatchFace::DrawInternal(expanded);
	DrawInternal(expanded);
}

void CDateWatchFace::StartDraw(CWatchyExpanded::ADisplay& display)
{
	display.setFullWindow();
	display.fillScreen(GxEPD_BLACK);
	display.setFont(&FreeMonoBold9pt7b);
	display.setCursor(0, 15);
}

void CDateWatchFace::DrawInternal(CWatchyExpanded& expanded)
{
	char buffer[32];
	strftime(buffer, sizeof(buffer), "%a %b %d, %Y", &expanded.Time());
	expanded.Display().println(buffer);
}
