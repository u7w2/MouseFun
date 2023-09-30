TODO:

Pre-Alpha:
- Detect all monitors `currently EnumDisplayMonitors() doesn't work on my machine`
- Fix Timing `use one thread for calculating, another thread for setting cursor and sleep`


Alpha:
- Using mouse applies a force on cursor's centre of mass `instead of an absolute shift in position, use f=ma`
- Use elasticity on bounces instead `pushing mouse right into the right wall creates force that compresses the cursor and bounces it back`
- Get current theme's main cursor, generate all rotations of it
- Calculate centre of mass of the cursor based on all opaque pixels
- Generate bounding box based on cursor's edges, store as vertices
- Bounce off vertices `calculate new trajectory based on rotation, vertex, and centre of mass`
  - Implement rotational inertia


Beta:
- Allow all cursor types `loading cursor and etc`
- Simulate aerodynamics `navier-stokes equation? use opencl for this if possible`
- Simulate density and buoyancy in fluid

Release:
- If cursor has two parts, detach the secondary part `like the main cursor + the loading ring`
  - Calculate secondary object's weight `based on pixels relative to primary part`
  - Apply collision physics to primary and secondary cursors separately, so primary and secondary can collide
    - On collision of primary and secondary, apply momentum based on weight of both
- Search for accelerometer devices. Instead of using artificial gravity, use the acceleration from the accelerometer.
  - Make sure it's based on the screen's angle, not the laptop base's angle `if laptop base is 90 degrees upright, and screen angle is 100 degrees (i.e. tilted down 10 degrees), cursor falls to top of screen`
- Elastic deformation of cursor is visible
