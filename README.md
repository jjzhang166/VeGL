<h1 align="middle"> VeGL </h1>
<p align="middle"><i> Graphics Engine</i></p>

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
 
  - [x] <b>Procedural ridged-multi-fractal</b> (absolute value of simplex-based fractal brownian motion) noise based terrain generation in Vertex shader. 
  - [x] <b>Procedural primitive generation</b> (<var>w</var>&times;<var>h</var> plane, 1 unit<sup>3</sup> cube)
  - [x] <b>Vertex specification</b> abstractions (for element/vertex array buffers and vertex arrays)
  - [x] <b>Transformation</b> framework with <b>quaternion</b>-based rotations
 &nbsp;  
  - [ ] <i>  More UI </i>
  - [ ] <i>  Refactoring, refactoring and &hellip; more refactoring! </i>
  - [ ] <i>  Realisting Lighting and then <b>PBR</b> </i>
  - [ ] <i>  On-the-fly shader generation for n-lights </i>
  - [ ] <i>  More transformation stuff (parents kind-of like in <b><important>Unity3D</b></important>) </i>
  - [ ] <i>  Entity component system and then <b><important>LUA</b></important> scripting (easy) </i>
  - [ ] <i>  Multiple viewports/cameras (using <code>glViewport</code> and <code>glScissor</code> and rendering multiple times) </i>
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
            <img src="Branding/screenshots/cur 12-27-2019.png" alt="Screenshot 12/27/2019">
        </section>
    </div> 
    <br>
    <hr>
    <footer>
        <p>Copyright &copy; 2019 <i><b>Nicolas Ortiz</i></b>. All rights reserved.</p>
    </footer>
</html>
