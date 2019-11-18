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
> .\vcpkg install --triplet x64-windows glew glfw sdl2 sdl2-image sdl2-mixer zeromq[sodium] czmq curl openexr imgui python3
```