# VeGL

<div>
  <nav>
    <p><b><important>Table of contents</important></b></p>
    <ul type = "disc">
      <li><i><small><a href="#Vendors">Vendors</a></small></i></li>
      <li><i><small><a href="#Features">Features</a></small></i></li>
    </ul>
  </nav> 
</div>
<hr>
<div id="Vendors">
  <section>
    <h3> Dependencies: </h2>
    <ul type = "square">
      <li> <b><important>GLFW</b></important> and <b><important>GLEW</b></important> for context creation and OpenGL access, respectively.</li>
      <li> <b><important>STB</b></important> single-header libraries for various tasks. </li>
      <li> <b><important>GLM</b></important> for maths </li>
      <li> <b><important>ImGUI</b></important> for UI </li>
    </ul>
  </section>
  <section>
    <h3> Everything else: </h2>
    <p>Copyright &copy; 2019 <i><b>Nicolas Ortiz</i></b>. All rights reserved.</p>
  </section>
</div>
<hr>
<div id="Features">
  <section>
    <h3> Current features: </h2>
    <ul type = "square">
      <li> <b>Procedural ridged-multi-fractal</b> (absolute value of simplex-based fractal brownian motion) noise based terrain generation in Vertex shader. </li>
      <li> <b>Procedural primitive generation</b> (<var>w</var>&times;z<var>h</var> plane, 1 unit<sup>3</sup> cube) </li>
      <li> <b>Vertex specification</b> abstractions (for element/vertex array buffers and vertex arrays) </li>
      <li> Transformation framework with <b>quaternion</b> rotations </li>
    </ul>
  </section>
  <section>
    <h3> To-do: </h2> 
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
  </section>
</div>
