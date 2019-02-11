#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "type-safety/Unit.hpp"

using namespace type_safety;
using namespace type_safety::unit_literals;

namespace /* anonymous */ {

using Tonnes = MassUnit<3>;

TEST(UnitTest, CompatibleTypeConversion) {
    constexpr auto mass = Value<Kilograms>{2.1f};

	constexpr auto kgs = Value<Kilograms>{mass};
    static_assert(floatEq(kgs.value<Tonnes>(), 0.0021f));
    static_assert(floatEq(kgs.value<Kilograms>(), 2.1f));
    static_assert(floatEq(kgs.value<Grams>(), 2100.0f));

    constexpr auto ts = Value<Tonnes>{mass};
	static_assert(floatEq(ts.value<Tonnes>(), 0.0021f));
	static_assert(floatEq(ts.value<Kilograms>(), 2.1f));
	static_assert(floatEq(ts.value<Grams>(), 2100.0f));

    constexpr auto gs = Value<Grams>{mass};
	static_assert(floatEq(gs.value<Tonnes>(), 0.0021f));
	static_assert(floatEq(gs.value<Kilograms>(), 2.1f));
	static_assert(floatEq(gs.value<Grams>(), 2100.0f));
}

TEST(UnitTest, LiteralsCreation) {
	static_assert(floatEq((4.2_kg).value<Kilograms>(), 4.2f));
	static_assert(floatEq((4_kg).value<Kilograms>(), 4.0f));

	static_assert(floatEq((4.2_g).value<Grams>(), 4.2f));
	static_assert(floatEq((4_g).value<Grams>(), 4.0f));
}

} // anonymous namespace
