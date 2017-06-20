----------------------------------------------# Dromophobia-game  --------------------------------------------------------
Cross the streets and collect the coins.

You are not allowed to cross streets in first 10 seconds. Do not get hit by the vehicles while you crossing the streets.
Collect the coins and try to make your best points ! 

Crossing line = 1 points
Collecting the coins = 5 points

--------------------------------------------------------------------------------------------------------------------------

If you want to contribute code to Dromophobia-game, I'm happy to help where you get stuck.

--------------------------------------------------------------------------------------------------------------------------

Installation and Build:

Installing OpenGL on Windows 10
The following instructions are for running OpenGL code using Microsoft Visual Studio Community 2017 IDE running 
on Windows 10 on 64-bit machines.However, the code should run, possibly with minor tweaks, using earlier versions 
of Visual Studio or Windows. For 32-bit machines, the instructions are the same except for the default 
system directory (noted below) and the default dependency directories (for headers and libraries) for Visual Studio.
Go to https://www.visualstudio.com/downloads and follow the links there to download (for free) and install Microsoft 
Visual Studio Community 2017. After Visual C++ has been successfully installed, do the steps below.

Step 1. Install OpenGL and GLU: OpenGL and GLU comes with Microsoft Windows. There are three main components:
1. Dynamic link library files opengl32.dll and glu32.dll should appear in the main system directory where the 
other .dll’s are stored (C:\Windows\SysWOW64 for 64-bit machines, C:\Windows\System32 for 32-bit machines.)
2. Lib files opengl32.lib and glu32.lib are stored somewhere accessible to your linker. (On my machine these 
appear in C:\Program Files (x86)\Windows Kits\10\Lib\10.0\um\x86, but yours may differ.) 
Let’s call this directory <MyLibraryDir>.
3. Definition files gl.h and glu.h are stored somewhere accessible to your C/C++ compiler. 
(On my machine, these appear in C:\Program Files (x86)\Windows Kits\10\Include\10.0\um\gl, but yours may differ.) 
Let’s call this directory <MyIncludeDir>.

Step 2. Install freeglut:
1. Download and unzip the freeglut 3.0.0 MSVC Package from
http://www.transmissionzero.co.uk/software/freeglut-devel/
2. Copy all the files from freeglut\include\GL to <MyIncludeDir> (wherever gl.h and glu.h are.)
3. Copy freeglut.lib from freeglut\lib to <MyLibraryDir> (wherever opengl32.lib and glu32.lib
are.)
4. Copy freeglut.dll from freeglut\bin to C:\Windows\SysWOW64. (or wherever opengl32.dll
and glu32.dll are.)
Important: Note that you can also download the 64-bit freeglut library if you are on a 64-bit machine, 
however it is okay to use the 32-bit freeglut library even if you are on a 64-bit machine. For some reason 
the 64-bit version of freeglut library did not work for me. So, I suggest that you use 32-bit freeglut library. 
Make sure you use 32-bit versions of both the linked library files (.lib) and their corresponding executables (.dlls). 
Otherwise there would be a mismatch and you would get a runtime error.
  
Step 3. Install GLEW:
1. Download glext.h from https://www.khronos.org/registry/OpenGL/api/GL/glext.h to <MyIncludeDir>
Download and unzip the file glew-2.0.0-win32.zip from http://glew.sourceforge.net/
2. Copy all the files from glew-2.0.0\include\GL to <MyIncludeDir>
3. Copy all the files from glew-2.0.0\lib\Release\Win32 to <MyLibraryDir>
4. Copy all the files from glew-2.0.0\bin\Release\Win32 to C:\Windows\SysWOW64
Important: Use 32-bit GLEW libraries even if you are on a 64-bit machine.
