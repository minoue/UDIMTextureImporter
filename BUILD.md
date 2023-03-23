# Build instruction

## Requirements to build
* [objModifier](https://github.com/minoue/objModifier)
* C++17 compiler


## How to build

1. Build and install libtiff.
2. Build objModifier. This requires libtiff.
3. Copy objModifier.exe to the DisplacementImporterData directory. You may also copy tiff.dll (libtiff.dylib) to DisplacementImporterData if you have built libtiff but do not want to install it to the system.
4. Build displacementImporter
5. Compile DisplacementImporter_2022.txt to zsc and move it ZPlugs64 directory.

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
ZPlugs64/
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
