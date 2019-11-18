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