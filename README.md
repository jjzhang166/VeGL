# VeGL
Graphics Engine, uses:
- GLFW and GLEW for context creation and OpenGL access.
- Stb single-header libraries for various tasks.
- GLM for maths
- ImGUI for UI

Everything else:
<p>Copyright &copy; 2019 Nicolas Ortiz. All rights reserved.</p>

Current Progress, features:
- Procedural ridged multi-fractal (absolute value of simplex-based fractal brownian motion) noise based terrain generation in Vertex shader.
- Procedural primitive generation (w*h plane, 1 unit^3 cube)
- Vertex specification abstractions (for element/vertex array buffers and vertex arrays)
- Transformation framework with quaternion rotations

Todo: 
- More ImGUI
- Refactoring... Refactoring and refactoring...
- Realisting Lighting and then PBR 
- On-the-fly shader generation for n-lights
- More transformation stuff (parents kinda like in Unity3D)
- Entity component system and then lua scripting (easy)
- Multiple viewports/cameras (using glviewport and glscissor and rendering multiple times)
- ....
- Skeletal animations
- Forward and inverse kinematics
- Btw: Already have done a lot of these things on other platforms (from scratch!) just have to implement them again, here.

