# Build instruction

## Requirements to build
* [objModifier](https://github.com/minoue/objModifier)
* C++17 compiler


## How to build

```
mkdir build
cd build
cmake ../
cmake --build . --config Release --target install
```

### Windows
1. Build and install libtiff.
2. Build objModifier. This requires libtiff.
3. Copy objModifier.exe to the DisplacementImporterData directory. You may also copy tiff.dll to DisplacementImporterData if you have built libtiff but do not want to install it to the system.
4. Build displacementImporter
5. Compile DisplacementImporter_2022.txt to zsc and move it ZPlugs64 directory.

#### Folder Hierarchy
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

### MacOS
wip

#### Folder Hierarchy
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
