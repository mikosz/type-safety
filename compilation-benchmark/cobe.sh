#!/bin/bash

FILES=100

cobe-generator/cobe_generator.py angle/angle.cpp.cobe angle/angle.h.cobe -o build/cobe/angle -n $FILES
cobe-generator/cobe_generator.py angle/angle-pch.cpp.cobe angle/angle-pch.h.cobe -o build/cobe/angle-pch -n $FILES
cobe-generator/cobe_generator.py angle/angle-float.cpp.cobe angle/angle-float.h.cobe -o build/cobe/angle-float -n $FILES

cobe-generator/cobe_generator.py unit/unit.cpp.cobe unit/unit.h.cobe -o build/cobe/unit -n $FILES
cobe-generator/cobe_generator.py unit/unit-pch.cpp.cobe unit/unit-pch.h.cobe -o build/cobe/unit-pch -n $FILES
cobe-generator/cobe_generator.py unit/unit-float.cpp.cobe unit/unit-float.h.cobe -o build/cobe/unit-float -n $FILES

cobe-generator/cobe_generator.py xform/xform.cpp.cobe xform/xform.h.cobe -o build/cobe/xform -n $FILES
cobe-generator/cobe_generator.py xform/xform-pch.cpp.cobe xform/xform-pch.h.cobe -o build/cobe/xform-pch -n $FILES
cobe-generator/cobe_generator.py xform/xform-matrix.cpp.cobe xform/xform-matrix.h.cobe -o build/cobe/xform-matrix -n $FILES
cobe-generator/cobe_generator.py xform/xform-matrix-pch.cpp.cobe xform/xform-matrix-pch.h.cobe -o build/cobe/xform-matrix-pch -n $FILES

../external/premake5.exe --scripts=../premake/ vs2017
