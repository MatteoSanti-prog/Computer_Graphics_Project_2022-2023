cd "C:\Users\santi\Documents\Visual Studio 2022\Projects\Computer_Graphics_Project_2022-2023"
cl /MD /I "C:\Users\santi\Documents\Visual Studio 2022\Libraries\glfw-3.3.8.bin.WIN64\include" /I .\headers\ /I C:\VulkanSDK\1.3.239.0\Include /std:c++17 /EHsc .\Main.cpp gdi32.lib opengl32.lib kernel32.lib user32.lib shell32.lib glfw3.lib vulkan-1.lib /link /LIBPATH:"C:\Users\santi\Documents\Visual Studio 2022\Libraries\glfw-3.3.8.bin.WIN64\lib-vc2022" /LIBPATH:C:\VulkanSDK\1.3.239.0\Lib
pause
