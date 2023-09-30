#include <Windows.h>
#include <WinUser.h>
#include <ShellScalingApi.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

struct monitor {
	RECT rect;
	int gravity;
};

static BOOL CALLBACK getMonitors(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
{
	std::vector<monitor>* monitors = (std::vector<monitor>*)pData;
	monitor newmon;
	newmon.rect = *lprcMonitor;

	UINT DPIx;
	UINT DPIy;
	GetDpiForMonitor(hMon, MDT_RAW_DPI, &DPIx, &DPIy);

	// 9.8067m/s^2 = 9.8067mm/ms^2
	// 1 DPI = 1/25.4 pixels per mm
	// x = DPI / 25.4 * 9.8067 = DPI / 2.59
	// x pixels per ms^2
	// tickrate * x = pixels per tick^2 = DPI * tickrate / 2.59
	// 
	// for realistic gravity at 9.8067m/s^2 it should be approx x2.7 at 144hz tickrate apparently, but this value seems way too high

	newmon.gravity = DPIy / 25;

	monitors->push_back(newmon);
	return 0;
}

int main(int argc, char* argv[]) {
	// this doesn't appear to detect all monitors. Fix
	std::vector<monitor> monitors;
	EnumDisplayMonitors(NULL, NULL, getMonitors, (LPARAM)&monitors);

	const int tickrate = 7;
	// const double drag = 1;
	const double bounciness = 0.8;

	int gravity = monitors[0].gravity;
	POINT pastCursor, presentCursor, futureCursor, velocity;
	GetCursorPos(&presentCursor);

	while (true) {
		pastCursor = presentCursor;
		GetCursorPos(&presentCursor);

		velocity.x = (presentCursor.x - pastCursor.x);
		velocity.y = (presentCursor.y - pastCursor.y + gravity);

		futureCursor.x = presentCursor.x + velocity.x;
		futureCursor.y = presentCursor.y + velocity.y;

		bool outside = true;
		int bouncewallX, bouncewallY;
		bool bounceX = false;
		bool bounceY = false;
		for (monitor const &mon : monitors) {
			if (futureCursor.x < mon.rect.left && presentCursor.x >= mon.rect.left) {
				bouncewallX = mon.rect.left;
				bounceX = true;
			}
			if (futureCursor.x > mon.rect.right && presentCursor.x <= mon.rect.right) {
				bouncewallX = mon.rect.right;
				bounceX = true;
			}
			if (futureCursor.y < mon.rect.top && presentCursor.y >= mon.rect.top) {
				bouncewallY = mon.rect.top;
				bounceY = true;
			}
			if (futureCursor.y > mon.rect.bottom && presentCursor.y <= mon.rect.bottom) {
				bouncewallY = mon.rect.bottom;
				bounceY = true;
			}

			if (futureCursor.x >= mon.rect.left && futureCursor.x <= mon.rect.right && futureCursor.y >= mon.rect.top && futureCursor.y <= mon.rect.bottom) {
				outside = false;
				gravity = mon.gravity;
			}
		}

		if (outside) {
			if (bounceX) {
				futureCursor.x = bouncewallX * 2 - futureCursor.x;
				presentCursor.x = futureCursor.x + velocity.x * bounciness;
			}
			if (bounceY) {
				futureCursor.y = bouncewallY * 2 - futureCursor.y;
				presentCursor.y = futureCursor.y + velocity.y * bounciness;
			}
		}
		SetCursorPos(futureCursor.x, futureCursor.y);
		std::this_thread::sleep_for(std::chrono::milliseconds(tickrate));
	}
	
	return 0;
}