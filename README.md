# VeGL

Graphics Engine

<h2> Dependencies: </h2>
<ul type = "square">
  <li> <b><important>GLFW</b></important> and <b><important>GLEW</b></important> for context creation and OpenGL access, respectively. </li>
  <li> <b><important>STB</b></important> single-header libraries for various tasks. </li>
  <li> <b><important>GLM</b></important> for maths </li>
  <li> <b><important>ImGUI</b></important> for UI </li>
</ul>

<h2> Everything else: </h2>
<p>Copyright &copy; 2019 Nicolas Ortiz. All rights reserved.</p>

<h2> Current Progress, features: </h2>

<ul type = "square">
  <li> Procedural ridged multi-fractal (absolute value of simplex-based fractal brownian motion) noise based terrain generation in Vertex shader. </li>
  <li> Procedural primitive generation (w*h plane, 1 unit^3 cube) </li>
  <li> Vertex specification abstractions (for element/vertex array buffers and vertex arrays) </li>
  <li> Transformation framework with quaternion rotations </li>
</ul>

<h2> Todo: </h2> 

<ol type = "a"> 
  <li><i>  More <b><important>ImGUI</b></important> </i></li>
  <li><i>  Refactoring... Refactoring and refactoring... </i></li>
  <li><i>  Realisting Lighting and then <b>PBR</b> </i></li>
  <li><i>  On-the-fly shader generation for n-lights </i></li>
  <li><i>  More transformation stuff (parents kinda like in Unity3D) </i></li>
  <li><i>  Entity component system and then <b><important>LUA</b></important> scripting (easy) </i></li>
  <li><i>  Multiple viewports/cameras (using <code>glViewport</code> and <code>glScissor</code> and rendering multiple times) </i></li>
  <li><i>  &hellip; </i></li>
  <li><i>  Skeletal animations </i></li>
  <li><i>  Forward and inverse kinematics </i></li>
</ol> 
