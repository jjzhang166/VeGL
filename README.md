<h1 align="middle"> VeGL </h1>
<p align="middle"><i> Physically Based Renderer/Engine</i></p>

 &nbsp;
 
<hr>
 
<html>
    <div>
        <nav>
        <p><b><important>Table of Contents</important></b></p>
        <ul type = "disc">
            <li><i><small><a href="#Vendors">Dependencies</a></small></i></li>
            <li><i><small><a href="#Features">Features</a></small></i></li>
            <li><i><small><a href="#Screen-shots">Screenshots</a></small></i></li>
        </ul>
        </nav> 
    </div>
    <hr>
    <br>
    <div id="Vendors">
        <section>
            <h3> Dependencies </h2>
            <ul type = "square">
                <li> <b><important><a href="https://www.glfw.org/">GLFW</a></b></important> and <b><important><a href="http://glew.sourceforge.net/">GLEW</a></b></important> for context creation and OpenGL access, respectively.</li>
                <li> <b><important><a href="https://github.com/nothings/stb">STB</a></b></important> single-header libraries for various tasks. </li>
                <li> <b><important><a href="https://glm.g-truc.net/0.9.9/index.html">GLM</a></b></important> for mathematics. </li>
                <li> <b><important><a href="https://github.com/ocornut/imgui">ImGUI</a></b></important> for UI. </li>
            </ul>
        </section>
    </div>
    <br>
    <hr>
    <br>
    <div id="Features">
        <section>
            <h3> Features </h2>
 
  - [x] <b>Procedural ridged-multi-fractal</b> (absolute value of simplex-based fractal brownian motion) noise based terrain generation in <del>Vertex shader</del> <ins>dedicated Compute shader </ins>.  
  - [x] <b>Procedural primitive generation</b> (<var>n<sup>2</sup> plane, 1 unit<sup>3</sup> cube)
  - [x] <b>Vertex specification</b> abstractions (for element/vertex array buffers and vertex arrays)
  - [x] <b>Transformation</b> framework with <b>quaternion</b>-based rotations
  - [x] <b>Compute-shader based procedural generation (increased uncapped framerate from 600fps to 3000+ fps, because we were recomputing the whole heightmap pixel by pixel every frame. Now, it's only done once.) 32-bit floating point heightmap precision. </b> 
 &nbsp;  
  - [x] <i>  Realisting Lighting and then <b>PBR</b> </i>
  - [~] <i>  More UI </i>
  - [~] <i>  Refactoring, refactoring and &hellip; more refactoring! </i>
  - [ ] <i>  n-lights </i>
  - [ ] <i>  More transformation stuff </i>
  - [ ] <i>  Entity component system and then <b><important>LUA</b></important> scripting (easy) </i>
  - [ ] <i>  Multiple viewports/cameras </i>
  - [ ] <i>  Skeletal animations </i>
  - [ ] <i>  Forward and inverse kinematics </i>
  
       </section>
    </div>
    <br>
    <hr>
    <br>
    <div id="Screen-shots">
        <section>
            <h3> Screenshots </h2>
            <img src="Branding/screenshots/wire.png" alt="Screenshot 12/28/2019">
            <img src="Branding/screenshots/now2.png" alt="Screenshot 12/29/2019">
            <img src="Branding/screenshots/now3.png" alt="Screenshot 12/29/2019">
            <img src="Branding/screenshots/screenshot.png" alt="Screenshot 1/17/2020">

       </section>
    </div> 
    <br>
    <hr>
    <footer>
        <p>VeGL Copyright &copy; 2019 <i><b>Nicolas Ortiz</i></b>. All rights reserved.</p>
    </footer>
</html>
