TODO:

Pre-Alpha:
- Detect all monitors `currently EnumDisplayMonitors() doesn't work on my machine`
- Fix infinite bounce


Alpha:
- Use floating points
- Using mouse applies a force on cursor's centre of mass `instead of an absolute shift in position, use f=ma`
  - Real mouse cursor hidden --- done
  - Current cursor icon used to render fake cursor
  - Distance travelled during tick is force
  - Upon measurement, cursor position resets to calculated icon position
- Use elasticity on bounces instead `pushing mouse right into the right wall creates force that compresses the cursor and bounces it back`
- Get current theme's main cursor, generate all rotations of it
- Calculate centre of mass of the cursor based on all opaque pixels
- Generate bounding box based on cursor's edges, store as vertices
- Bounce off vertices `calculate new trajectory based on rotation, vertex, and centre of mass`
  - Implement rotational inertia
- Allow all cursor types `loading cursor and etc`


Beta:
- Simulate aerodynamics `navier-stokes equations? use opencl for this`
- Simulate density and buoyancy in fluid


Release:
- If cursor has two parts, detach the secondary part `like the main cursor + the loading ring`
  - Calculate secondary object's weight `based on pixels relative to primary part`
  - Apply collision physics to primary and secondary cursors separately, so primary and secondary can collide
    - On collision of primary and secondary, apply momentum based on weight of both
- Search for accelerometer devices. Instead of using artificial gravity, use the acceleration from the accelerometer.
  - Make sure it's based on the screen's angle, not the laptop base's angle `if laptop base is 90 degrees upright, and screen angle is 100 degrees (i.e. tilted down 10 degrees), cursor falls to top of screen`
- Elastic deformation of cursor is visible
- Clicking applies friction to screen `like pressing cursor against wall`
  - Force applied while pressed is converted to elastic potential energy and is release when mouse is released
