# ILCTool

Open-source software for interlaboratory studies and proficiency testing.

## Overview

ILCTool is a C++ application for conducting interlaboratory studies according to international standards.

### Planned Support

- ISO 13528 (Proficiency Testing – Robust statistics, z-score)
- ASTM E691 (Precision Study – Sr, SR, r, R)
- ISO 5725 (Accuracy studies)

## Current Status

Early development phase.
Core domain structure implemented.

## Build (Windows – MSYS2)

Requirements:
- MSYS2 (UCRT64)
- g++

Build:

```bash
g++ -std=c++17 -g -Idomain -Iutils ilctool.cpp -o build/ILCTool.exe
```

Run:
```bash
build/ilctool.exe
```

License:
MIT License. See LICENSE file for details.