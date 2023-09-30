TODO:

urgent:
then make it detect all monitors (currently EnumDisplayMonitors() doesn't seem to work on my machine)
fix timing - use one thread for calculating, another thread for setting cursor and sleep


next step:

moving the mouse doesn't move the mouse, it applies a force on its centre of mass in the direction of its movement
get current theme's main cursor, generate all rotations of it
calculate centre of mass of the cursor based on all opaque pixels
generate bounding box based on cursor's edges, store as vertices
rework the collisions so that it bounces off the vertices and calculates new trajectory based on centre of mass
	use entire mass of cursor to figure out its speed of spin - implement rotational inertia


to implement all physics:

simulate aerodynamics of flying cursor (navier-stokes equation?) based on the cursors trajectory, speed, and shape - use opencl for this
simulate friction on solid surfaces (monitor walls). i.e. simulate grip, so at certain angle the cursor may slip instead of
 bouncing, but once it regains grip it can high-side like a motorbike
simulate density and buoyancy in fluid

to be completely extra:

allow all cursor types to change between each other (loading cursor and etc)
if cursor has two parts (like the main cursor + the loading ring), detach the secondary part
calculate secondary object's weight based on pixels relative to primary part
apply collision physics to secondary and primary cursors separately, so primary and secondary can collide
	on collision of primary and secondary, apply momentum based on weight of both
search for accelerometer devices. Instead of using artificial gravity, use the acceleration from the accelerometer.
make sure it's based on the screen's angle, not the laptop base's angle, so if laptop base is 90 degrees upright,
and screen angle is 100 degrees, so it's pointing 10 degrees down, cursor falls to top of screen