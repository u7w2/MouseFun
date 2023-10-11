#define OEMRESOURCE
#include <Windows.h>
#include <WinUser.h>
#include <ShellScalingApi.h>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iostream>
#include "config.h"
#include "fakeCursor.h"

static HCURSOR realHCursors[13];

void cleanUp() {
	setRealCursors(realHCursors);
}

struct monitor {
	RECT rect;
	int gravity;
};

BOOL CALLBACK getMonitors(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
{
	std::vector<monitor>* monitors = (std::vector<monitor>*)pData;
	monitor newmon;
	newmon.rect = *lprcMonitor;

	UINT DPIx;
	UINT DPIy;
	GetDpiForMonitor(hMon, DPIMode, &DPIx, &DPIy);

	// 9.8067m/s^2 = 9.8067mm/ms^2
	// 1 DPI = 1/25.4 pixels per mm
	// x = DPI / 25.4 * 9.8067 = DPI / 2.59
	// x pixels per ms^2
	// tickrate * x = pixels per tick^2 = DPI * tickrate / 2.59
	// 
	// for realistic gravity at 9.8067m/s^2 it should be approx x2.7 at 144hz tickrate apparently, but this value seems way too high

	newmon.gravity = DPIy * gravityScalar;

	monitors->push_back(newmon);
	return 0;
}

int main(int argc, char* argv[]) {
	// this doesn't appear to detect all monitors. Fix
	std::vector<monitor> monitors;
	EnumDisplayMonitors(NULL, NULL, getMonitors, (LPARAM)&monitors);

	POINT pastCursor, presentCursor, futureCursor, velocity;
	GetCursorPos(&presentCursor);

	// create ticker class
	auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(tickrate);
	while (false) {
		pastCursor = presentCursor;
		GetCursorPos(&presentCursor);

		velocity.x = (presentCursor.x - pastCursor.x);
		velocity.y = (presentCursor.y - pastCursor.y + monitors[0].gravity);

		futureCursor.x = presentCursor.x + velocity.x;
		futureCursor.y = presentCursor.y + velocity.y;

		bool outside = true;
		int bouncewallX, bouncewallY;
		bool bounceX = false;
		bool bounceY = false;
		for (std::vector<monitor>::iterator mon = monitors.begin(); mon != monitors.end(); mon++) {
			if (futureCursor.x < mon->rect.left && presentCursor.x >= mon->rect.left) {
				bouncewallX = mon->rect.left;
				bounceX = true;
			}
			if (futureCursor.x > mon->rect.right && presentCursor.x <= mon->rect.right) {
				bouncewallX = mon->rect.right;
				bounceX = true;
			}
			if (futureCursor.y < mon->rect.top && presentCursor.y >= mon->rect.top) {
				bouncewallY = mon->rect.top;
				bounceY = true;
			}
			if (futureCursor.y > mon->rect.bottom && presentCursor.y <= mon->rect.bottom) {
				bouncewallY = mon->rect.bottom;
				bounceY = true;
			}

			if (futureCursor.x >= mon->rect.left && futureCursor.x <= mon->rect.right && futureCursor.y >= mon->rect.top && futureCursor.y <= mon->rect.bottom) {
				outside = false;
				std::rotate(monitors.begin(), mon, mon + 1);
				break;
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

		std::this_thread::sleep_until(x);
		x += std::chrono::milliseconds(tickrate);
	}

	getRealHCursors(realHCursors);
	atexit(cleanUp);

	HCURSOR noCursors[13];
	getNoHCursors(noCursors, L"nocursor.cur");
	HDC screenDC = GetDC(NULL);

	HDC compatDC = CreateCompatibleDC(screenDC);
	HBITMAP compatBMP = CreateCompatibleBitmap(screenDC, 32, 32);
	//setRealCursors(noCursors);

	SelectObject(compatDC, compatBMP);
	HDC tmpDC = CreateCompatibleDC(compatDC);
	HBITMAP tmpBMP = CreateCompatibleBitmap(compatDC, 32, 32);

	DrawIcon(compatDC, 0, 0, realHCursors[0]);
	for (int p = 0; p < 150; p++) { // eh
		BitBlt(compatDC, 0, 0, 32, 32, screenDC, 100 + (p << 3), 100 + (p << 3), SRCCOPY);
		DrawIcon(compatDC, 0, 0, realHCursors[0]);
		SelectObject(tmpDC, tmpBMP);
		BitBlt(screenDC, 100 + (p - 1 << 3), 100 + (p - 1 << 3), 32, 32, tmpDC, 0, 0, SRCCOPY);
		BitBlt(tmpDC, 0, 0, 32, 32, screenDC, 100 + (p << 3), 100 + (p << 3), SRCCOPY);
		SelectObject(compatDC, compatBMP);
		BitBlt(screenDC, 100 + (p << 3), 100 + (p << 3), 32, 32, compatDC, 0, 0, SRCCOPY);
		//Sleep(1);
	}
	return 0;
	DrawIconEx(compatDC, 200, 300, realHCursors[0], 32, 32, 0, 0, DI_NORMAL | DI_DEFAULTSIZE);
	Sleep(1000);
	DrawIconEx(compatDC, 300, 300, realHCursors[0], 32, 32, 0, 0, DI_NORMAL | DI_DEFAULTSIZE);
	Sleep(1000);
	DrawIconEx(compatDC, 300, 400, realHCursors[0], 32, 32, 0, 0, DI_NORMAL | DI_DEFAULTSIZE);
	Sleep(1000);
	DrawIconEx(compatDC, 400, 400, realHCursors[0], 32, 32, 0, 0, DI_NORMAL | DI_DEFAULTSIZE);

	return 0;
}