# SSPHH 2019

This is the repository for my 2019-2020 work on SSPHH.

## Build Directions

* Install the libraries using [vcpkg](https://github.com/Microsoft/vcpkg).
* Download [Fluxions](https://github.com/microwerx/fluxions).
* Download [SSPHH](https://github.com/microwerx/ssphh-all).
* Download Visual Studio 2019
* Put repos in common area such as `C:\github`
  * `C:\github\vcpkg`
  * `C:\github\fluxions`
  * `C:\github\ssphh-all`
* Open `ssphh-all.sln` and compile

## Libraries

```
PS Z:\github\vcpkg> .\vcpkg.exe install --triplet x64-windows openexr czmq curl sdl2-mixer sdl2-image imgui sdl2 glew python3 zeromq[sodium] glfw3 freeglut
```

## Overview of Operation

- [ ] Geometry Template Engine (GTE)
- [ ] Render Config Manager
- [ ] Simple Scene Graph
- [ ] GLES 2.0 Renderer
- [ ] GLES 3.0 Renderer

## Current Status

`SSPHH_Application` is a `Vf::Widget` which is decorated by `Vf::DearImGuiWidget` and `Vf::Widget`. But a better way would use a `Vf::RootWindow` as a container which steps through several `Vf::Widget` children.

## TODO

- [ ] Add Viperfish/ImGui Controls
  - [ ] `VfWidget()`
  - [ ] `VfWindow(const std::string&, int w, int h)`
  - [ ] `VfCheckbox(const std::string&, bool)`
  - [ ] `VfOptions(const std::string&, std::vector<std::string>)`
  - [ ] `VfSlider<T>(const std::string&, T x, T a, T b)`
  - [ ] `VfSliderf = VfSlider<float>`
  - [ ] `VfSlideri = VfSlider<int>`
  - [ ] `VfGraph(const std::string&, float xmin, float xmax, float ymin, float ymax)`
- [ ] Add a `FX_LoadingScreenWindow`
- [ ] Add a `FX_PerformanceWindow` to display frame rate and other statistics
- [ ] Add a `FX_RenderConfigWindow` to display information about loaded render configs 
- [ ] Add a `FX_SceneGraphWindow` to display information about the loaded scene graph
- [ ] Add a `SG_NodeWindow` to display node hierarchy and matrix transformations
- [ ] Add a `SG_GeometryWindow` to display information about loaded 3D models
- [ ] Add a `SG_LightWindow` to display information about lights in the scene
  - [ ] `MoonInfo`
  - [ ] `SunInfo`
  - [ ] `EnviroInfo`
- [ ] Add a `SG_EnviroMapWindow` to display an cube map image
  - [ ] `GammaOptions({ "None", "Remove", "sRGB" })`
  - [ ] `AddGammaCheckbox(false)`
  - [ ] `ToneMapSlideri(0, -12, 12)`
  - [ ] `SaturateCheckbox(false)`
- [ ] Add a `SG_TextureMapWindow` to display all loaded texture maps
  - [ ] Same options from `SG_EnviroMapWindow`
