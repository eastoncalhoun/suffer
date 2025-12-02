# Suffer - C/C++ Package Manager

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
suffer install https://github.com/fmtlib/fmt
suffer install https://github.com/gabime/spdlog
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
- <a href="https://github.com/doctest/doctest">doctest</a> - doctest
- <a href="https://github.com/CLIUtils/CLI11">CLI11</a> - CLIUtils
- <a href="https://github.com/CrowCpp/Crow">Crow</a> - CrowCpp
- <a href="https://github.com/yhirose/cpp-httplib">cpp-httplib</a> - yhirose
- <a href="https://github.com/marzer/tomlplusplus">toml++</a> - marzer
- <a href="https://github.com/SRombauts/SQLiteCpp">SQLiteCpp</a> - SRombauts
- <a href="https://github.com/SOCI/soci">soci</a> - SOCI
- <a href="https://github.com/jtv/libpqxx">pqxx</a> - jtv

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
- g++ (C++17)
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
├── include/       # Headers (project's)
└── suffer.json    # Dependency configuration
```

# No more suffering with dependencies!
Results may vary. Currently, specific version control is unsupported. I plan to work on that in the future at some point. I have also not tested it with boost at all. Plan to get more libraries working with it first.

## License

GNU GPL v3