cl /MD /I C:\computer_graphics\glfw-3.3.8.bin.WIN64\include /I .\headers\ /I C:\computer_graphics\VulkanSDK\1.3.239.0\Include /std:c++20 /EHsc .\Main.cpp gdi32.lib opengl32.lib kernel32.lib user32.lib shell32.lib glfw3.lib vulkan-1.lib /link /LIBPATH:C:\computer_graphics\glfw-3.3.8.bin.WIN64\lib-vc2022 /LIBPATH:C:\computer_graphics\VulkanSDK\1.3.239.0\Lib
