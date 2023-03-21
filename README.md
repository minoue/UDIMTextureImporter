# Displacement Importer for ZBrush

UDIM normal/vector displacement importer


https://user-images.githubusercontent.com/7100231/226717384-3f47a9d0-e34f-4216-8190-48a2260320f9.mp4


## Requirements
* [objModifier](https://github.com/minoue/objModifier)
* C++17

## Install

### Windows10
Download [zip](https://github.com/minoue/displacementImporter/releases/download/v0.1/DisplacementImporter_2022_win10.zip) file and extract it.
Move 'displacementImporterData' folder and DisplacementImporter_2022.zsc to ZPlug64 directory.

### MacOS
...


## How to build

#### MacOS
```
clang++ -dynamiclib -std=c++17 -current_version 1.0 -fvisibility=hidden -o displacementImporter.lib displacementImporter.cpp
```

#### Windows
```
mkdir build
cd build
cmake ../
cmake --build . --config Release --target install
```
