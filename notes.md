This is to keep todo free of clutter

what's the best way of getting a cursor drawn on the screen?
windows GDI seems ok ish, drawicon seems to have a black background atm for some reason
when you have to separate the cursor into two fakes, is it better to use two cursors, or not use gdi? idek


yeahahhhhhhhh I don't think I'll stick to WinAPI any more. SDL seems like a good option for drawing, Vulkan would be hella overkill but fun
I can load the cursor icons another way, perhaps a way that's easier to control. Obviously taking over the Windows cursor must use WinAPI, but that's not a problem. I'll leave the complex stuff to the complex libraries, and windows stuff to WinAPI