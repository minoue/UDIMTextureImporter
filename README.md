# UDIM Texture Importer for ZBrush

https://user-images.githubusercontent.com/7100231/226717384-3f47a9d0-e34f-4216-8190-48a2260320f9.mp4

## Notes:

**This early version may still have bugs. It is recommended to save the file under a different name and use a new layer. It is not guaranteed to work in all environments.**

## Supported textures

### Vector Displacement

Textures need to be... 

* Tangent Vector
* 32bit tiff/exr or 16bit float-point exr
* Mid point 0.0
* UDIM naming convention (eg. fileName.1001.tif)
* Flip and switch: 25 (ZBrush)
* Tangent Flip and switch: 25 (ZBrush)
* Absolute tangent (Mudbox)

### Normal Displacement
* 32bit tiff/exr or 16bit float-point exr
* Mid point 0.0
* UDIM naming convention (eg. fileName.1001.tif)

### Color
* 8 or 16 bit tiff
* UDIM naming convention (eg. fileName.1001.tif)

### B/W mask
* 8 or 16 bit tiff
* UDIM naming convention (eg. fileName.1001.tif)

**There are no settings on the UI to change the above settings at the moment. Therefore, any maps that do not follow these settings will not work.**

## Install

### Windows10 & ZBrush2022
1. Download the latest [UDIMTextureImporter for Win]() file and extract it to ZPlug64 folder.
2. Move tiff.dll to the same directory as ZBrush.exe.
3. Move UDIMTextureImporter.zsc to ZPlug64 folder

### MacOS & ZBrush2022
1. Install libtiff : `brew install libtiff`
1. Download the latest [UDIMTextureImporter for MacOS]() file and extract it to ZPlug64 folder.
3. Move UDIMTextureImporter.zsc to ZPlug64 folder

## Build Instruction
### Requirements
* C++17
* [libtiff](http://www.libtiff.org)

For MacOS, you can use homebrew: `brew install libtiff`

For Windows, you may use a package manager such as Chocolatey or build from source code.

### How to build

```sh
git clone https://github.com/minoue/displacementImporter
cd displacementImporter
git submodule update --init --recursive
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
cmake --build . --config Release --target install
```

## Credits
This software uses the following libraries.

[tinyexr](https://github.com/syoyo/tinyexr) / The 3-Clause BSD License / Shoyo Fujita

[libtiff](http://www.libtiff.org) / LibTIFF license / Copyright © 1988-1997 Sam Leffler / Copyright © 1991-1997 Silicon Graphics, Inc.  

[FromZ](https://github.com/n-taka/FromZ) / GPLv3  / © Kazutaka Nakashima   

[Eigen](https://eigen.tuxfamily.org/) / MPL2 

## License
