# [C++ Verion] Pixel-wise-Color-Tracking-using-Particle-Filter

Description: Tracking color object by using only one pixel, requring object has a single color. The main component of this method is based on particle filtering. The particle filtering is well explained in my Matlab Version. 

Prerequisite: 
  - OpenCV 3.1.0 
  - Installing X11 on Centos 7.2 and Xming on Window for remote displaying images ( which is in my case). You dont need to install these packages if working personal PC with mornitor.
  - cmake 2.8.11
  - GNU 4.8.5

Tested Environment: C++ on CentOS 7.2. 

How to compile: 
- >> Extract compressed file. 
- >> cd "unzip folder"
- >> mkdir build 
- >> cmake ../ 
- >> make 
- >> ./main 

How to use: 
1. Replace image path in the main.cpp. The code is only working with set of frames instead of reading video directly. Thus, you should extract video into set of frames. 
2. When the image is diplayed, I have written a function on_mouse to capture the mouse click pressed and released. Thus, using your mouse to draw a rectangle. Note that you will not see the bounding box immediately, but then when your press keyboard, the rectange will be displayed. 
Now, enjoying the program. 
3. THe rest of program, you just need to press keyboard, since I used waitkey(0) in my program. 

Now, hope you enjoy
