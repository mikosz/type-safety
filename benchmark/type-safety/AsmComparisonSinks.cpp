#include "type-safety/Unit.hpp"
#include "type-safety/Angle.hpp"

using namespace type_safety;

using Joulles = decltype(Kilograms{} * Metres{} * Metres{} / (Seconds{} * Seconds{}));
using Energy = Value<Joulles>;

float externalAngleSink(
	[[maybe_unused]] Angle pitch,
	[[maybe_unused]] Angle yaw,
	[[maybe_unused]] Angle roll
) {
	return 0.0f;
}

float externalAngleFloatSink(
	[[maybe_unused]] float pitch,
	[[maybe_unused]] float yaw,
	[[maybe_unused]] float roll
) {
	return 0.0f;
}

float externalUnitSink([[maybe_unused]] Energy e) {
	return e.value<Joulles>();
}

float externalUnitFloatSink([[maybe_unused]] float f) {
	return f;
}
