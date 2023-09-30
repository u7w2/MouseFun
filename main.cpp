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
	newmon.gravity = DPIy / 25; // for realistic gravity at 9.8067m/s^2 it should be ~ x2.7 at 144hz tickrate

	monitors->push_back(newmon);
	return 0;
}

int main(int argc, char* argv[]) {
	std::vector<monitor> monitors;
	
	EnumDisplayMonitors(NULL, NULL, getMonitors, (LPARAM)&monitors);

	const int tickrate = 7;
	const double drag = 1;
	const double bounciness = 0.8;

	int gravity = monitors[0].gravity;
	POINT pastCursor, presentCursor, futureCursor, velocity;
	GetCursorPos(&presentCursor);

	while (true) {
		pastCursor = presentCursor;
		GetCursorPos(&presentCursor);

		velocity.x = (presentCursor.x - pastCursor.x) * drag;
		velocity.y = (presentCursor.y - pastCursor.y + gravity) * drag;

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

/* TODO:
* 
* urgent:
* fix multiple monitors - make it functional on multiple rectangles - done
   then make it detect all monitors (currently EnumDisplayMonitors() doesn't seem to work)
* fix timing - use one thread for calculating, another thread for setting cursor and sleep
* 
* 
* next step:
* 
* moving the mouse doesn't move the mouse, it applies a force on its centre of mass in the direction of its movement
* get current theme's main cursor, generate all rotations of it
*	calculate centre of mass of the cursor based on all opaque pixels
*	generate bounding box based on cursor's edges, store as vertices
*	rework the collisions so that it bounces off the vertices and calculates new trajectory based on centre of mass
*	use entire mass of cursor to figure out its speed of spin - implement rotational inertia
* 
* 
* to implement all physics:
* 
* simulate aerodynamics of flying cursor (navier-stokes equation?) based on the cursors trajectory, speed, and shape - use opencl for this
* simulate friction on solid surfaces (monitor walls). i.e. simulate grip, so at certain angle the cursor may slip instead of
   bouncing, but once it regains grip it can high-side like a motorbike
* simulate density and buoyancy in fluid
* 
* to be completely extra:
* 
* allow all cursor types to change between each other (loading cursor and etc)
* if cursor has two parts (like the main cursor + the loading ring), detach the secondary part
*	calculate secondary object's weight based on pixels relative to primary part
*	apply collision physics to secondary and primary cursors separately, so primary and secondary can collide
* 	on collision of primary and secondary, apply momentum based on weight of both
* search for accelerometer devices. Instead of using artificial gravity, use the acceleration from the accelerometer.
*	make sure it's based on the screen's angle, not the laptop base's angle, so if laptop base is 90 degrees upright,
	 and screen angle is 100 degrees, so it's pointing 10 degrees down, cursor falls to top of screen
* 
*/