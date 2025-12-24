# Suffer - Suffering free C++ Package Manager

A simple, decentralized package manager for C++, that is similar to npm.

## The Problem

C++ dependency management is a nightmare. CMake is turing complete config, vcpkg requires Microsoft's blessing, Conan drags Python into everything, and manual library management makes one consider... Rust.

Most learning developers manually copy-paste headers and pray.

```bash
# The manual way (what Suffer automates)

# Clone libraries
git clone https://github.com/fmtlib/fmt
git clone https://github.com/gabime/spdlog

# Build fmt
cd fmt
cmake -S . -B build -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cd ..

# Build spdlog with external fmt
cd spdlog
cmake -S . -B build -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release -DSPDLOG_FMT_EXTERNAL=ON
cmake --build build --config Release
cd ..

# Set up project
mkdir -p project/src project/include project/lib project/out

# Copy headers
cp -r fmt/include/fmt project/include/
cp -r spdlog/include/spdlog project/include/

# Copy static libs
cp fmt/build/libfmt.a project/lib/
cp spdlog/build/libspdlog.a project/lib/
```
## The Solution

Suffer makes it marginally less painful.

```bash
suffer install spdlog
suffer install fmt
suffer import spdlog
```

Done. No writing XML, Gradle, Cmake.. Just JSON.

## Core Philosophy

**Decentralized** — Install from any Git repository. No central registry, no corporate overlords.

**Static by default** — Dynamic linking is how good friendships end. Self-contained binaries just work.

**JSON only** — Zero scripting, zero DSLs, zero Lua. Configuration is simple JSON or nothing.

**Minimal workflow** — Install, import, make. That's it.

## How It Works

1. `suffer install <github-url OR known package OR --local>` Registers the package
2. `suffer import <package>` builds it (if needed), copies headers to `./include`, static libs to `./lib`, and updates your project config
3. `make` compiles your project with the right flags

### Example
[![Video](https://img.youtube.com/vi/MSdjSqQtVVw/0.jpg)](https://youtu.be/MSdjSqQtVVw)

Suffer tracks dependencies, handles link order, and caches compiled libraries so you only build once.

## Supported Libraries

Suffer ships with configurations for common libraries:
- <a href="https://github.com/nlohmann/json/">json</a> - nlohmann
- <a href="https://github.com/fmtlib/fmt/">fmt</a> - fmtlib
- <a href="https://github.com/catchorg/Catch2">Catch2</a> - catchorg
- <a href="https://github.com/gabime/spdlog">spdlog</a> - gabime
- <a href="https://github.com/raysan5/raylib">raylib</a> - raysan5
- <a href="https://github.com/glfw/glfw">glfw</a> - glfw
- <a href="https://github.com/Tencent/rapidjson">rapidjson</a> - Tencent
- <a href="https://github.com/simdjson/simdjson">simdjson</a> - simdjson
- <a href="https://github.com/libcpr/cpr">cpr</a> - libcpr
- <a href="https://github.com/pantor/inja">inja</a> - pantor
- <a href="https://github.com/doctest/doctest">doctest</a> - doctest
- <a href="https://github.com/CLIUtils/CLI11">CLI11</a> - CLIUtils
- <a href="https://github.com/CrowCpp/Crow">Crow</a> - CrowCpp
- <a href="https://github.com/yhirose/cpp-httplib">cpp-httplib</a> - yhirose
- <a href="https://github.com/marzer/tomlplusplus">toml++</a> - marzer
- <a href="https://github.com/SRombauts/SQLiteCpp">SQLiteCpp</a> - SRombauts
- <a href="https://github.com/SOCI/soci">soci</a> - SOCI
- <a href="https://github.com/jtv/libpqxx">pqxx</a> - jtv
- <a href="https://github.com/jedisct1/libsodium">libsodium</a> - jedisct1

Adding more configurations pre-package install, is just a JSON entry in `~/.suffer/known.json`.
Post install the configuration file is `~/.suffer/libs/lib/suffer.json`.

## Example Config File (suffer.json for spdlog)
```js
{
    "package": "spdlog",
    "author": "gabime",
    "flags": "-DSPDLOG_FMT_EXTERNAL=ON",
    "source": "https://github.com/gabime/spdlog",
    "version": "1.16.0",
    "headerOnly": false,
    "dependencies": {
        "fmt": "any"
    }
}
```

## Installation

### Requirements
- g++ (C++17+)
- cmake
- make
- pkg-config
- git
- linux or wsl

```bash
git clone https://github.com/eastoncalhoun/suffer
cd suffer
make
make install
```

## Commands
```
suffer help        Show help
suffer install     Install a package from GitHub
suffer import      Import a package into the current directory
suffer list        Show package information
suffer uninstall   Remove a package
suffer clean       Clear suffer's build cache
suffer init        Initialize a new project structure
suffer regen       Regenerates a project's Makefile
```

## Project Structure

Suffer generates:
```
your-project/
├── src/           # Your source files
├── include/       # Headers (dependencies)
├── lib/           # Static libraries
├── out/           # Build output
├── suffer.project.json    # linking configuration
└── Makefile       # build/run
```

## Creating a Suffer Library

Suffer expects:
```
your-project/
├── src/           # Your source files
├── include/       # Headers (project's, dependencies)
└── suffer.json    # Dependency configuration
```

## Tested Linux Distributions:
- **Debian 12 x86_64 Headless** - Currently running a production webserver built with suffer
- **Arch Linux x86_64 KDE** - All listed libraries tested and working
- **Gentoo Linux x86_64 KDE** - Working (raylib may require additional system configuration)
- **Other Distros** - System dependency warnings will indicate what to install via your package manager for packages with known configurations

# No more suffering with dependencies!
Results may vary. Currently, specific version control is unsupported. I plan to work on that in the future at some point. The current roadmap for new features in no particular order is as followed:
- Better `suffer.project.json` - Right now it does weird stuff if you import the same library more than once. It also should ideally contain much more useful metadata - Soon
- Custom compile script support per lib's `suffer.json` - When suffer goes to compile a library to static, instead of defaulting to wrapping around cmake or g++, if a lib has a script specified, use it. If you put a shebang at the top of whatever script linked I think any interperated lang could be supported pretty painlessly
- Actual version control via git chrono - `unknown` is good enough for my uses, but it'd be nice to have specific version flags besides `sys` and `any` - Other features are more important to me right now
- `suffer clean packagename` - Clean the cache for specific packages - soon cause global only is getting old 
- `suffer clean --deep` - Clean out all build files from inside the lib dir itself. - May just do a reinstall command, cause it accomplishes the same thing
- `suffer build cmake` - CMake project config generation (suffer project -> CMake project) - This one is a little trickier, and I'm pretty sure it'll be very difficult. May or may not happen, I'm pretty sure I could auto generate a lot of the CMake but not all of it. And at that point you may as well be using CMake anyways or are already using it.

## License

GNU GPL v3

## Example Project
<a href="https://github.com/eastoncalhoun/budget">Minimal Server Rendered Webpage</a>
