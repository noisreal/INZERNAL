# INZERNAL
Growtopia internal cheat focused around enhancements and framework, that is loaded automatically.

## Currently in development, do not make pull requests

### Information
* Not to be confused with "internal", the first internal ever made for growtopia, that was developed by atipls, me, and tero. 
* That is our private internal and will not be released.
* INZERNAL uses some small parts from it, but otherwise this project is fully made by me.
* Patcher coded by ness


### Building yourself
1. Download project
1. Make sure you are using Release x64
1. (Optional) Modify source if you want to
1. Build all projects
1. Choose your method of injection
   * injector (See below)
   * patcher (See below)
   * Any custom injector
   * Both injector and patcher

### Using the injector
* Keep INZERNAL.dll in the same directory
* F1 will inject INZERNAL to Growtopia
* F3 will uninject INZERNAL and restore everything (Built into INZERNAL)
* F4 will quit injector
* Requires admin permissions due to growtopia changing token permissions

### Using the patcher
* Drag any unpatched Growtopia file into patcher
* Put INZERNAL.dll into same directory as Growtopia
* When you launch Growtopia, INZERNAL.dll is automatically loaded
* Does not require admin perms like injector
* **You can use injector with patched file too**

### Verified compatible Growtopia versions
* Some patterns might be outdated for some versions
* Find versions [here](https://github.com/ama6nen/INZERNAL/tree/main/gt)
* Verified working Growtopia versions are
  * V.41 (Recommended version)
  * V3.39
  * V.32

### Downloads
* No need to build anything yourself
* **No support for custom settings right now (will support later)**
* [Downloads here](https://github.com/ama6nen/INZERNAL/releases)


### Dependencies used in this project 
#### Note that none of these need to be downloaded, they are included in the project
* [libpebliss](https://github.com/asinbow/libpebliss)
* [minhook](https://github.com/TsudaKageyu/minhook)
* [json](https://github.com/nlohmann/json)
* [miniz](https://github.com/richgel999/miniz)
* [proton](https://github.com/SethRobinson/proton) (small parts + modified)
* [imgui](https://github.com/ocornut/imgui) (modified)
