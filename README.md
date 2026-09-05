# Retro-Sonic 3D Decompilation

A complete decompilation of Retro-Sonic 3D: Sky Worlds.

Without assets from the Retro-Sonic 3D 2005 demo, this decompilation will not run. You can download the game [here](https://archive.org/details/3dtest_202105).

# Additional Tweaks
* Added a built in mod loader, allowing to easily create and play mods.
* Added an OpenGL + SDL/2/3 renderer, replacing the original DirectX 7 rendering code.

# How to Build

This project uses [CMake](https://cmake.org/), a versatile building system that supports many different compilers and platforms. You can download CMake [here](https://cmake.org/download/). **(Make sure to enable the feature to add CMake to the system PATH during the installation!)**

## Get the source code

**DO NOT** download the source code ZIP archive from GitHub, as they do not include the submodules required to build the decompilation.

Instead, you will need to clone the repository using Git, which you can get [here](https://git-scm.com/downloads).

Clone the repo **recursively**, using:
`git clone --recursive https://github.com/RSDKModding/RSonic3D-Decompilation`

If you've already cloned the repo, run this command inside of the repository:
```git submodule update --init --recursive```

## Getting dependencies

### Windows
To handle dependencies, you'll need to install [Visual Studio Community](https://visualstudio.microsoft.com/downloads/) (make sure to install the `Desktop development with C++` package during the installation) and [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-cmd#1---set-up-vcpkg) (You only need to follow `1 - Set up vcpkg`).

After installing those, run the following in Command Prompt (make sure to replace `[vcpkg root]` with the path to the vcpkg installation!):
- `[vcpkg root]\vcpkg.exe install glew sdl3 --triplet=x64-windows-static` (If you're compiling a 32-bit build, replace `x64-windows-static` with `x86-windows-static`.)

Finally, follow the [compilation steps below](#compiling) using `-DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_PREFIX_PATH=[vcpkg root]/installed/x64-windows-static/` as arguments for `cmake -B build`.
  - Make sure to replace each instance of `[vcpkg root]` with the path to the vcpkg installation!
  - If you're compiling a 32-bit build, replace each instance of `x64-windows-static` with `x86-windows-static`.

### Linux
Install the following dependencies: then follow the [compilation steps below](#compiling):
- **pacman (Arch):** `sudo pacman -S base-devel cmake glew sdl3`
- **apt (Debian/Ubuntu):** `sudo apt install build-essential cmake libglew-dev libsdl2-dev`
- **rpm (Fedora):** `sudo dnf install make gcc gcc-c++ cmake glew-devel SDL3-devel`
- **apk (Alpine/PostmarketOS)** `sudo apk add build-base cmake glew-dev sdl3-dev`
- Your favorite package manager here, [make a pull request](https://github.com/RSDKModding/RSonic3D-Decompilation/fork)

## Compiling

Compiling is as simple as typing the following in the root repository directory:
```
cmake -B build
cmake --build build --config release
```

The resulting build will be located somewhere in `build/` depending on your system.

The following cmake arguments are available when compiling:
- Use these by adding `-D[flag-name]=[value]` to the end of the `cmake -B build` command. For example, to build with `RETRO_ORIGINAL_CODE` set to on, add `-DRETRO_ORIGINAL_CODE=on` to the command.

### RSonic3D flags
- `RETRO_FORCE_CASE_INSENSITIVE`: Forces case insensivity when loading files. Takes a boolean, defaults to `off`.
- `RETRO_MOD_LOADER`: Enables or disables the mod loader. Takes a boolean, defaults to `on`.
- `RETRO_ORIGINAL_CODE`: Removes any custom code, enabling the original DirectX 7 renderer and menu bar. *The DirectX 7 SDK is required and will only target Windows x86.* Takes a boolean, defaults to `off`.
- `RETRO_SDL_VERSION`: *Only change this if you know what you're doing.* Switches between using SDL1, SDL2 or SDL3. Takes an integer of either `1`, `2` or `3`, defaults to `3`.

# Contact:
Join the [Retro Engine Modding Discord Server](https://dc.railgun.works/retroengine) for any extra questions you may need to know about the decompilation or modding it.