# Suffer - C/C++ Package Manager
A simple, decentralized package manager for C and C++ that actually works.

## The Problem
<summary>
C++ dependency management is a nightmare. CMake is what hell is like, vcpkg is a cry for help, Conan injects Python just to feel something, and manual library management makes one contemplate.. <details>Rust...</details> Again.. Most developers just copy-paste headers or give up entirely and move to a higher level language.
</summary>

## The Solution

Suffer makes it... Marginally less painful.

```bash
suffer install https://github.com/author/package 
#OR
suffer local-install /path/to/package.tar.gz

suffer import package
make
```

Done. No configuration files to write, no build system integration, no dependency hell. No pain! From dependency management..

## Core Philosophy

### Decentralization
Install from any Git repository, no central registry gods or masters.

### Static linking by default
Dynamic linking is how good friendships come to an end.
Self-contained binaries that just work, no runtime dependency nightmares.

### JSON only
Zero scripting, zero DSL, zero Lua.. Not Gradle... If you need to configure something, it's simple JSON.

### One command workflows
Suffer's import command handles everything! It builds installed dependencies if they aren't cached, copies headers to ./lib; it even generates basic Makefiles (WOW). 

## Key Features:

- Automatic dependency detection and resolution
- Build caching to avoid recompiling
- Project isolation (similar to Python's venv or Node's npm. No system-wide pollution)
- Automatic Makefile generation
- Future: AppImage/Flatpak packaging support

## Installation

### Requirements
- Git
- GCC/Clang with C++17 support
- GNU Make

```bash
git clone https://github.com/eastoncalhoun/suffer/

cd ./suffer

make

sudo make install #🥴 trust me bro
```
# No more suffering!
Results may vary

## License
GNU GPL v3