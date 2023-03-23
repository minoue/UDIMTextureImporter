# Build instruction

## Requirements to build
* [objModifier](https://github.com/minoue/objModifier)
* C++17 compiler


## How to build

1. Build libtiff first.
2. Build objModifier.
3. Copy **objModifier.exe** and **tiff.dll** to DisplacementImporterData directory.
4. Build displacementImporter
5. Compile DisplacementImporter_2022.txt to zsc

#### MacOS
```
cd src
clang++ -dynamiclib -std=c++17 -current_version 1.0 -fvisibility=hidden -o displacementImporter.lib displacementImporter.cpp
```

#### Windows
```
mkdir build
cd build
cmake ../
cmake --build . --config Release --target install
```

## Folder Hierarchy
The folder structure should look like this

#### Windows
```
ZPlugs64/
├─ DisplacementImporter_2022.zsc
├─ DisplacementImporterData/
│  ├─ displacementImporter.dll
│  ├─ objModifier.exe
│  ├─ tiff.dll
│  ├─ ZFileUtils/
│  │  ├─ ZFileUtils64.dll
```

#### MacOS
```
ZPlug64/
├─ DisplacementImporter_2022.zsc
├─ DisplacementImporterData/
│  ├─ displacementImporter.lib
│  ├─ objModifier
│  ├─ lib/
│  │  ├─ libtiff.dylib
│  │  ├─ libtiff.6.dylib
│  │  ├─ libtiff.6.0.0.dylib
│  ├─ ZFileUtils/
│  │  ├─ ZFileUtils.lib
```
