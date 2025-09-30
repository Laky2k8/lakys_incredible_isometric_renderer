# Laky's Phenomenal Pathtracer
<br>

Old RPG games like Fallout 1 and 2 and other isometric games always fascinated me: The way they convey three dimensions with no perspective and relatively simple maths always mesmerised me. But my main problem was always that despite the simple math all of these engines always remained
using prerendered graphics; So I set out to change that and with some tutorials, some internet and AI help and a LOT of banging my head to the wall, I can proudly say that I have achieved something that is pretty goddamn cool :D
<br>

# Features
- OBJ loading support (via https://github.com/Bly7/OBJ-Loader/)
- Full texture support
- Shading
- Fully controllable camera
- High FPS (I mean who doesn't love that amirite :P)

The engine itself is also deliberately made to be renderer-independent, so with very little change you can port it to any renderer you like! By default the project uses Raylib.

<br>

# Screenshots

<img width="802" height="632" alt="grassblock" src="https://github.com/user-attachments/assets/dce4f5ba-06b6-4a39-aad8-5e13201ff14c" />

<img width="802" height="632" alt="teapot1" src="https://github.com/user-attachments/assets/06903efc-4149-45a1-8319-f781bd3c88d9" />

<img width="802" height="632" alt="teapot2" src="https://github.com/user-attachments/assets/b32681b7-ff02-4be7-a444-2bae710861b5" />

<br>

# How to build
Just set up a C++ development environment with Raylib (or if you port it your preferred renderer). Then just build the project with any compiler you like. There is only one requirement: The used renderer must be passed as a flag, for example <code>-DUSE_RAYLIB</code> for Raylib.
**WARNING: I only tested the code on Windows. I can not guarantee that it will build on Linux or Mac OS, but it probably should :)**

# How to run
To run the engine, just download the newest release, unzip and launch `IsometricEngine.exe`.

