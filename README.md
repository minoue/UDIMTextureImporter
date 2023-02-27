# Displacement Importer for ZBrush (for MacOS for now)

UDIM normal/vector displacement importer


## Requirements
[objModifier](https://github.com/minoue/objModifier)

## How to build

For MacOS

```
clang++ -dynamiclib -std=c++11 -current_version 1.0 -fvisibility=hidden -o displacementImporter.lib displacementImporter.cpp
```

For Windows
```
-
```
