This is to keep todo free of clutter

what's the best way of getting a cursor drawn on the screen?
windows GDI seems ok ish, drawicon seems to have a black background atm for some reason
when you have to separate the cursor into two fakes, is it better to use two cursors, or not use gdi? idek


yeahahhhhhhhh I don't think I'll stick to WinAPI any more. SDL seems like a good option for drawing, Vulkan would be hella overkill but fun
I can load the cursor icons another way, perhaps a way that's easier to control. Obviously taking over the Windows cursor must use WinAPI, but that's not a problem. I'll leave the complex stuff to the complex libraries, and windows stuff to WinAPI

GLFW probably best... AYYOOOO LMAO I'm GONNA USE VULKAN AAHAHAHAHAHAHAHAHAHA

https://www.youtube.com/watch?v=BIZyxja3Qls an option

I have to create rotations of the cursor using opencv anyway, might as well start there by loading the bitmap in instead, could be easier
do I need opencv? is it bloat?







so I'm sticking to WinAPI still apparently. Maybe something will change when I have to generate rotations of every cursor... ugh


----------
I need to allow animated cursors to work somehow FUCK