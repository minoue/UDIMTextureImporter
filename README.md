# Displacement Importer for ZBrush

UDIM normal/vector displacement importer


<iframe src="https://player.vimeo.com/video/809951157?h=3208d1af24" width="640" height="360" frameborder="0" allow="autoplay; fullscreen; picture-in-picture" allowfullscreen></iframe>
<p><a href="https://vimeo.com/809951157">UDIM Vector Displacement Importer for ZBrush</a> from <a href="https://vimeo.com/user4337917">Michi Inoue</a> on <a href="https://vimeo.com">Vimeo</a>.</p>


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
