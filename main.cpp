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
	/*monitor tmp;
	tmp.rect.left = 0;
	tmp.rect.top = 0;
	tmp.rect.right = 1280;
	tmp.rect.bottom = 720;
	tmp.gravity = 5;*/
	std::vector<monitor> monitors;
	/*monitors.push_back(tmp);
	tmp.rect.left = 1280;
	tmp.rect.top = 400;
	tmp.rect.right = 1880;
	tmp.rect.bottom = 1060;
	monitors.push_back(tmp);*/
	
	EnumDisplayMonitors(NULL, NULL, getMonitors, (LPARAM)&monitors);

	const int tickrate = 7;
	const double drag = 1;
	const double bounciness = 0.8;

	int gravity = monitors[0].gravity;
	POINT prevCursor, crCursor, velocity;
	GetCursorPos(&crCursor);

	while (true) {
		prevCursor = crCursor;
		GetCursorPos(&crCursor);

		velocity.x = (crCursor.x - prevCursor.x) * drag;
		velocity.y = (crCursor.y - prevCursor.y + gravity) * drag;

		int nextx = crCursor.x + velocity.x;
		int nexty = crCursor.y + velocity.y;

		// scans through all monitors
		bool outside = true;
		int bouncewall;
		bool xy;
		for (monitor const &mon : monitors) {
			if (nexty > mon.rect.bottom) {
				bouncewall = mon.rect.bottom;
				xy = true;
				continue;
			}
			if (nextx > mon.rect.right) {
				bouncewall = mon.rect.right;
				xy = false;
				continue;
			}
			if (nextx < mon.rect.left) {
				bouncewall = mon.rect.left;
				xy = false;
				continue;
			}
			if (nexty < mon.rect.top) {
				bouncewall = mon.rect.top;
				xy = true;
				continue;
			}
			gravity = mon.gravity;
			outside = false;
		}
		if (outside) {
			if (xy && (velocity.y > 10)) { // actually fix this stupid infinite loop instead of using this bs hack
				nexty = bouncewall * 2 - nexty;
				crCursor.y = nexty + velocity.y * bounciness / drag;
			}
			else if (!xy) {
				nextx = bouncewall * 2 - nextx;
				crCursor.x = nextx + velocity.x * bounciness;
			}
		}

		SetCursorPos(nextx, nexty);
		std::this_thread::sleep_for(std::chrono::milliseconds(tickrate));
	}
	
	return 0;
}

/* TODO:
* 
* urgent:
* fix multiple monitors - make it functional on multiple rectangles
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