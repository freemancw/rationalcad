![Logo](http://freemancw.com/junk/rcad-logo-2.png)

Integers and rational numbers offer an attractive alternative to floating-point arithmetic from a robustness point of view. RationalCAD explores the use of these number types for solid modeling with convex polytopes.

#### Get Started ####

RationalCAD is composed of a [[geometry library]] and the actual GUI program. While the geometry library has minimal dependencies, the GUI is built using the Qt framework. Thus, most developers will prefer to work within Qt Creator. This guide to compiling is built with these users in mind. RationalCAD uses CMake to structure the compilation process. 

##### Windows x64

1. Clone the repository. `git clone https://github.com/freemancw/rationalcad.git `
1. Install prerequisite software.
  * [Visual Studio Community 2013](http://www.visualstudio.com/en-us/products/free-developer-offers-vs)
  * [Qt 5.5 64-bit](http://www.qt.io/download-open-source/)
  * [CMake 2.8.12 or later](http://www.cmake.org/download/) (Choose to add `cmake` to your path for all users.)
1. Open `rationalcad/CMakeLists.txt` in QtCreator. ![Open in QtCreator](http://freemancw.com/junk/rationalcad-qtcreator-cmakelists.png)
1. Choose the build location. Typically this should be an out-of-source folder (e.g. if the RationalCAD source is in `C:\workspace\rationalcad` then the build folder is `C:\workspace\rationalcad-build`). ![Choose build location](http://freemancw.com/junk/rationalcad-qtcreator-build-location.png)
1. Run CMake with default settings. ![Run CMake](http://freemancw.com/junk/rationalcad-qtcreator-run-cmake.png)
1. Choose `Build -> Build All`. ![Build all](http://freemancw.com/junk/rationalcad-qtcreator-build-all.png)
1. Finally, click the `Run` button (the green sideways arrow on the left side of the screen). ![Run](http://freemancw.com/junk/rationalcad-qtcreator-run.png)

