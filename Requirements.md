# Build tools #

The project uses CMake for now. You'll need at least CMake 2.4.x. You can download the latest version on the CMake download page: http://cmake.org/HTML/Download.html

## Windows ##

You'll need a C++ compiler. Any version of Visual Studio should work although the code was only tested on Visual 2005. Alternatively, you can install the free MinGW or the free command line compiler from Microsoft.
  * Microsoft Visual C++ 2005 Express Edition: http://msdn.microsoft.com/vstudio/express/visualc/default.aspx
> > Don't forget to install the platform SDK
  * MinGW: http://www.mingw.org/ You'll need at least:
    * g++
    * gcc-core ( I think )
    * binutils

Could someone with a plain Windows try one of those and update the requirements?

> (A. I was going to do this but chickened out. But didn't gamer2k4 get it going? How did that go, gamer2k4, can you record the results here?)
> (C.) I got that last one working by installing Dev-C++ which comes with a complete instalation of MinGW so, if all else fails, just install that one ( version 5.0 beta IIRC )

# External libraries #

There are only a few of them, all SDL related:
  * SDL
  * SDL\_image with PNG support
  * SDL\_net
  * SDL\_mixer
  * SDL\_TTF

Warning for Windows users: Download the devel version of the libs to get the requires .h and .lib files. If you are using MinGW and not Visual Studio, download the MinGW version of those.