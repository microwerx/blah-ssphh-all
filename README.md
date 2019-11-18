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
- [ ] Add a `LoadingScreenWindow`
- [ ] Add a `SG_EnviroMapWindow` to display an cube map image
  - [ ] `GammaOptions({ "None", "Remove", "sRGB" })`
  - [ ] `AddGammaCheckbox(false)`
  - [ ] `ToneMapSlideri(0, -12, 12)`
  - [ ] `SaturateCheckbox(false)`
- [ ] Add a `SG_TextureWindow` to display all loaded texture maps