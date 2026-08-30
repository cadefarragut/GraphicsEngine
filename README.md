
# Roman Colosseum — Real-Time OpenGL Renderer

A small real-time 3D renderer written from scratch in **C++ and OpenGL 3.3**, with no
game engine. It renders a ~1,500-block model of the Roman Colosseum under a Phong
lighting pipeline with an animated day/night cycle.

<img width="480" height="270" alt="gif" src="https://github.com/user-attachments/assets/9ff5d5dc-3e91-4252-a51e-517ae91961dd" />


<img width="1916" height="1076" alt="image" src="https://github.com/user-attachments/assets/2effdbe1-b115-48d1-8d2c-4fcc16991b9b" />


<img width="1921" height="1081" alt="image" src="https://github.com/user-attachments/assets/0b7a8061-3e3c-41e5-9eef-a519caef72e2" />


## Features

- **Lighting** — Phong (ambient / diffuse / specular) with two directional lights,
  a warm sun and a cool sky-fill, summed through a `CalcDirLight` helper.
- **Materials** — each texture id has its own tint, shininess and specular strength.
- **Day/night cycle** — the sun sweeps across the sky on a timer; its position and
  colour and the sky-gradient are all driven by the sun's height.
- **Procedural sky** — a vertex-coloured gradient cube that follows the camera,
  drawn unlit behind the scene.
- **Data-driven scenes** — the whole environment is a plain-text list of boxes
  (`map.txt`: centre, size, texture id), loaded at runtime and decoupled from
  engine code.
- Small wrapper classes over the OpenGL vertex-array / buffer / shader / texture
  objects.

## Built with

- C++20, OpenGL 3.3 core
- [GLFW](https://www.glfw.org/) (windowing / input), [glad](https://glad.dav1d.de/) (GL loader)
- [GLM](https://github.com/g-truc/glm) (math), [stb_image](https://github.com/nothings/stb) (texture loading)

All dependencies are vendored under `vendor/`, so the project builds without any
external setup.

## Build & run (Windows / Visual Studio)

1. Open `personalproj.slnx` in Visual Studio 2022.
2. Select the **Debug | x64** configuration.
3. Build and run (F5).

The working directory must be the repository root so the shaders (`src/assets/*.vs`,
`*.fs`) and textures load by relative path — this is the default when running from
Visual Studio.

Command-line build:

```
msbuild personalproj.vcxproj /p:Configuration=Debug /p:Platform=x64
```

## Controls

| Key | Action |
| --- | --- |
| `W` `A` `S` `D` | Move |
| Mouse | Look |
| `Shift` / `Ctrl` | Up / down |
| Scroll wheel | Zoom (FOV) |
| `Esc` | Quit |

## Project layout

```
src/
  main.cpp            window setup, main loop, input
  Config.h            screen size constants
  core/window.*       GLFW window wrapper
  game/camera.*       fly camera (LearnOpenGL-style)
  render/
    render.*          the renderer: lighting, day/night, scene + sky + sun draws
    VAO/VBO/EBO.*     OpenGL buffer wrappers
    shader.*          shader program loader + uniform helpers
    Texture.*         texture loader + specular-map generator
  assets/             shaders (.vs/.fs) and textures (.jpg)
map.txt               the scene: one textured box per line
vendor/               GLFW, glad, GLM, stb_image
```

## Notes

This started as a walk-through of the [LearnOpenGL](https://learnopengl.com/)
*Getting Started* and *Lighting* chapters and grew from there. It deliberately
stays within those techniques, so there is no shadow mapping, HDR/tone mapping or
MSAA, and the geometry is axis-aligned boxes rather than curved surfaces — those
would be the natural next steps.
