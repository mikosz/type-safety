#!/bin/bash

cobe-generator/cobe-generator/cobe_generator.py angle.cpp.cobe angle.h.cobe -o angle -n 20
cobe-generator/cobe-generator/cobe_generator.py angle-pch.cpp.cobe angle-pch.h.cobe -o angle-pch -n 20
cobe-generator/cobe-generator/cobe_generator.py angle-float.cpp.cobe angle-float.h.cobe -o angle-float -n 20

../external/premake5.exe --scripts=../premake/ vs2017
