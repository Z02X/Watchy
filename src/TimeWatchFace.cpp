// Self
#include "TimeWatchFace.h"

// Fonts
#include <Fonts/FreeMonoBold9pt7b.h>

// Time
#include <TimeLib.h>

void CTimeWatchFace::Draw(CWatchyExpanded& expanded)
{
	StartDraw(expanded.Display());
	DrawInternal(expanded);
}

void CTimeWatchFace::StartDraw(CWatchyExpanded::ADisplay& display) const
{
	display.setFullWindow();
	display.fillScreen(GxEPD_BLACK);
	display.setFont(&FreeMonoBold9pt7b);
	display.setCursor(0, 15);
}

void CTimeWatchFace::DrawInternal(CWatchyExpanded& expanded)
{
	CWatchyExpanded::ADisplay& display = expanded.Display();
	const std::tm& time = expanded.Time();

	display.print("Time: ");
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%I:%M %p", &time);
	display.println(buffer);
}
