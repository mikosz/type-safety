#!/bin/bash

cobe-generator/cobe_generator.py angle/angle.cpp.cobe angle/angle.h.cobe -o build/cobe/angle -n 20
cobe-generator/cobe_generator.py angle/angle-pch.cpp.cobe angle/angle-pch.h.cobe -o build/cobe/angle-pch -n 20
cobe-generator/cobe_generator.py angle/angle-float.cpp.cobe angle/angle-float.h.cobe -o build/cobe/angle-float -n 20

../external/premake5.exe --scripts=../premake/ vs2017
