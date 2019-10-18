<h1>Gravity Render Engine v2.0</h1>

> Current state of the engine will not build for this branch.

<p>Gravity is a hobby render engine that is being developed with the idea of learning graphics programming and engine development written entirely in C++ ... and a little bit of C. The engine is currently in heavy development and architecture will change over time.
<br><br>Gravity's development philosophy revolves around modularity and every feature written into the engine has been given ample amounts of consideration and thinking before finalising the API's design and structure. You will also note that Gravity tries to avoid C++'s std library whenever possible if you're looking inside of it's codebase. This was done simply to maximise learning experience as I try to recreate important basic data structures (Array, String and etc ...) and fine tune them to be as performant as the standard's.<br><br>The current end-game goal for Gravity is to implement Physically Based Rendering (PBR) into Gravity's renderer.</p>
<br/>

> Note, `development` is being heavily updated with `master` getting updates from time to time.

<br/>

<h2>Specification</h2>


<table style="width:100%">
  <tr>
    <td><b>Platform</b></td>
    <td>Windows 10 (x64)</td>
  </tr>
  <tr>
    <td><b>Graphics API</b></td>
    <td>OpenGL 4.5</td>
  </tr>
</table>
<br>

<h2>How to build?</h2>


No external build process tools were used for this project. Hence simply follow the following steps:

1. Clone th project.

```
   git clone https://github.com/yeongasm/grvt-engine.git
```
2. Open Visual Studio.
3. In Visual Studio, File -> New -> Project From Existing Code ...

<br/>
<h2>Roadmap</h2>


<p>The following are features that are planned to be released into the engine:</p>
<p>Phase 1:</p>

- [ ] Add in remaining basic graphic features; framebuffers & cubemaps.
- [ ] Have a wrapper class around Window's threading API.
- [ ] Make the renderer multi-threaded.
- [ ] Implement PBR.
- [ ] Save project to JSON and able to load project JSON into Gravity.
- [ ] Rewrite the String class.

<p>Phase 2:</P>

- [ ] Coming soon ...

<br/>
<h2>Screenshot(s)</h2>


<br/>
<img src="Devlog/Pictures/Annotation 2019-08-13 000820.png" alt="img"/>

<br/>
<h2>External Dependencies</h2>


<ul>
<li><a href="https://github.com/ocornut/imgui">dear imgui</a></li>
<li><a href="https://github.com/assimp/assimp">Open Asset Import Library (assimp)</a></li>
<li><a href="https://github.com/nothings/stb">stb</a></li>
<li><a href="https://github.com/mlabbe/nativefiledialog">nativefiledialog</a></li>
</ul>
