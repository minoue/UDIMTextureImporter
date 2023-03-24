# Displacement Importer for ZBrush

UDIM Normal/Vector displacement importer


https://user-images.githubusercontent.com/7100231/226717384-3f47a9d0-e34f-4216-8190-48a2260320f9.mp4

## Notes:

**This early version may still have bugs. It is recommended to save the file under a different name and use a new layer. It is not guaranteed to work in all environments.**

**Because of a technical limitation, this plugin needs to export a temporary obj file and it may take some time if your mesh has more than a few million polygons. You might want to take a cup of tea or coffee.**


## Requirements for maps
Displacement maps need to be...

* Tangent Vector or standard displacement
* 32bit tiff/exr or 16bit float-point exr
* Mid point 0.0
* named as UDIM, such as body.1001.tif

and exported in the follwing settings for the tangent vector.

#### ZBrush
* Flip and switch: 25
* Tangent Flip and switch: 25

#### Mudbox
* Absolute tangent

**There are no settings on the UI to change the above settings at the moment. Therefore, any maps that do not follow these settings will not work.**

## Install

#### Windows10 & ZBrush2022
Download [zip](https://github.com/minoue/displacementImporter/releases/download/v0.1/DisplacementImporter_2022_win10.zip) file and extract it.

Move 'displacementImporterData' folder and DisplacementImporter_2022.zsc to the ZPlug64 directory.


#### MacOS & ZBrush2022
Coming soon.


## Build
[How to build](./BUILD.md)


## Credits
This software uses the following libraries.

[CLI11: Command line parser for C++11](https://github.com/CLIUtils/CLI11) / The 3-Clause BSD License / Henry Schreiner

[tinyexr](https://github.com/syoyo/tinyexr) / The 3-Clause BSD License / Shoyo Fujita

[libtiff](http://www.libtiff.org) / LibTIFF license / Copyright © 1988-1997 Sam Leffler / Copyright © 1991-1997 Silicon Graphics, Inc.  

[objModifier](https://github.com/minoue/objModifier) / MIT License / Michitaka Inoue

## License
[MIT License](./LICENSE)



