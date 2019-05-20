#include "type-safety/Unit.hpp"

using namespace type_safety;
using namespace type_safety::unit_literals;

using Joulles = decltype(Kilograms{} * Metres{} * Metres{} / (Seconds{} * Seconds{}));
using Energy = Value<Joulles>;

float externalUnitSink(Energy e);
float externalUnitFloatSink(float f);

namespace unit_test {

float callUnit(Mass m, Speed v) {
	return externalUnitSink(m * v * v * 0.5f);
}

float callFloats(float m, float v) {
	return externalUnitFloatSink(m * v * v * 0.5f);
}

} // namespace unit_test
