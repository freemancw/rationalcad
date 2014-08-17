RationalCAD
===========

#### Robust polyhedral modeling ####

Analysis of what is happening in floating-point implementations is often muddled by the fact that they do not possess the basic properties of associativity and closure under any arithmetic operations. In contrast, integers are closed under addition and multiplication (forming a ring), and rational numbers are closed under addition, multiplication, and division (forming a field). Assuming we do not run out of memory, we can treat these implementations like we do the mathematical concepts of ring and field.

Arbitrary precision (AP) integers and rational numbers offer an attractive alternative to floating-point arithmetic from a robustness point of view. RationalCAD explores the use of these number types for solid modeling with convex polytopes.

#### Compiling ####

1. Install prerequisite software
  * Visual Studio 2013
  * CMake 2.8.12
  * Qt 5.3 32-bit (Desktop OpenGL)
1. Compile MPIR in dependencies/mpir
1. Open RationalCAD in QtCreator
