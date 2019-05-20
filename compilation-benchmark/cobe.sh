#!/bin/bash

cobe-generator/cobe_generator.py angle/angle.cpp.cobe angle/angle.h.cobe -o build/cobe/angle -n 100
cobe-generator/cobe_generator.py angle/angle-pch.cpp.cobe angle/angle-pch.h.cobe -o build/cobe/angle-pch -n 100
cobe-generator/cobe_generator.py angle/angle-float.cpp.cobe angle/angle-float.h.cobe -o build/cobe/angle-float -n 100

cobe-generator/cobe_generator.py unit/unit.cpp.cobe unit/unit.h.cobe -o build/cobe/unit -n 100
cobe-generator/cobe_generator.py unit/unit-pch.cpp.cobe unit/unit-pch.h.cobe -o build/cobe/unit-pch -n 100
cobe-generator/cobe_generator.py unit/unit-float.cpp.cobe unit/unit-float.h.cobe -o build/cobe/unit-float -n 100

../external/premake5.exe --scripts=../premake/ vs2017
