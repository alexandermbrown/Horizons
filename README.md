# Horizons
This repository contains a game and game engine prototype developed by Alex Brown as a hobby project. The project is a work in progress and serves as a place for me to build my understanding and skills.

## Overview
`Lithium/` - A game engine written in C++.

`AssetBase/` - A basic asset compiler/packer for `Lithium`.

`Horizons/` - A game prototype which uses the `Lithium` engine.

### Lithium
Lithium was originally based off an early state of the [Hazel Engine](https://github.com/TheCherno/Hazel). This was simply to give me a boost in understanding how to structure the basics of an engine. Since then, I have added dozens of features and have adapted the code base to my needs and understanding. Features of the engine include:
- an API agnostic OpenGL / DirectX 11 [renderer](Lithium/src/Lithium/Renderer)
- A multi-channel SDF [text renderer](Lithium/src/Lithium/Renderer/Font.cpp) using [msdfgen](https://github.com/Chlumsky/msdfgen) and [HarfBuzz](https://github.com/harfbuzz/harfbuzz)
- 3D Audio using OpenAL
- packed assets built with `AssetBase`
- a multithreaded [video player](Lithium/src/Lithium/VideoPlayer) using FFmpeg

### AssetBase
AssetBase is the asset compiler / formatter / packer for `Lithium`. It uses [Google FlatBuffers](https://google.github.io/flatbuffers/) to serialise and pack the assets into files. It features:
- [msdf](https://github.com/Chlumsky/msdfgen) font generation
- [ShaderConductor](https://github.com/microsoft/ShaderConductor) to translate HLSL to GLSL ([code](AssetBase/src/AssetBase/Resources/ShaderSegment.cpp))

### Horizons
Horizons is a prototype game built with Lithium. It features:
- a [level editor](Horizons/src/Horizons/LevelEditor) using ImGui
- basic 2D top-down terrain rendering
- ImGui overlays (console, performance diagnostics, config viewer)
- Lua UI [scripting](Horizons/src/Horizons/Scripting)
- an entity component system using [entt](https://github.com/skypjack/entt)
- [Box2D](https://box2d.org/) physics
- two persistent threads, one for rendering and one for game logic

## Setup
The project currently only supports 64-bit Windows systems.
### Visual Studio 2019
Open PowerShell and run `bin/Win-Setup.ps1`
