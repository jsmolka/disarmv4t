# Building
A C++17 compliant compiler is required on all platforms.

## Clone
Use [git](https://git-scm.com/) to clone the repository and its submodules.

```
> git clone https://github.com/jsmolka/disarmv4t
> cd disarmv4t
> git submodule update --init
```

## Build

### Windows
Build the Visual Studio solution. This can also be done on the command line.

```
> call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
> msbuild /property:Configuration=Release disarmv4t.sln
```

### Linux / macOS
Build with [cmake](https://cmake.org/).

```
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-march=native" ..
$ make -j 4
```
