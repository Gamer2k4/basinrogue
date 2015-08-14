# Windows #

Here is how I build the Windows version. First, make sure that all [Requirements](Requirements.md) are installed of course.

  * Open a terminal and change directory into the basinrogue\cpp folder.
  * Make sure that SDLDIR, SDLIMAGEDIR, SDLMIXERDIR, SDL\_TTFDIR and SDLNETDIR point to the install path of the development versions of those libs.
> > _Bug in some CMake versions older than 2.4.6 : CMake might not find parts of SDL\_net because of a bug in the detect script. To work around that bug, set both SDLNETDIR and SDLNET to point to the SDL\_net install path_
  * Type the <cmake .> command. There should be no errors.
  * Build using NMake
  * If the above doesn't work, it's probably because you need to inform CMake on the type of secondary build environment. For example, to generate a Visual Studio 2005 project file  and build from here, you can run cmake with the -G "Visual Studio 8 2005" option. Use cmake without any other option to see the available values for that option.


# Unix or MacOS X #

  * Follow the same instructions as in Windows, but you probably do not need to set the Environment vars to point to the SDL libs install path.
> > If CMake complains that it cannot find one of the SDL libs, most often it means that your Distro split appart the libSDL header files into a separate devel package. Check your distro package manager for those additional installs.


(A. I can successfully build the Windows version, using a batch file to pass Cmake the relevant paths. However, the resulting workspace doesn't come up right in MSVC++ - the client project does not show up on the left hand pane, only the server project. Vexing.)