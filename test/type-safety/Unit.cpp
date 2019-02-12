#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "type-safety/Unit.hpp"

using namespace type_safety;
using namespace type_safety::unit_literals;

namespace /* anonymous */ {

using Tonnes = MassUnit<1000, 1>;
using Pounds = MassUnit<45359, 100000>;

TEST(UnitTest, DistanceCompatibleTypeConversion) {
    constexpr auto distance = Value<Metres>{2.1f};

	constexpr auto ms = Value<Metres>{distance};
    static_assert(floatEq(ms.value<Kilometres>(), 0.0021f));
    static_assert(floatEq(ms.value<Metres>(), 2.1f));

	constexpr auto kms = Value<Kilometres>{distance};
    static_assert(floatEq(kms.value<Kilometres>(), 0.0021f));
    static_assert(floatEq(kms.value<Metres>(), 2.1f));
}

TEST(UnitTest, MassCompatibleTypeConversion) {
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

	constexpr auto lbs = Value<Pounds>{2.2046f};
	static_assert(floatEq(lbs.value<Kilograms>(), 1.0f));
}

TEST(UnitTest, TimeCompatibleTypeConversion) {
    constexpr auto time = Value<Seconds>{2.1f};

	constexpr auto secs = Value<Hours>{time};
    static_assert(floatEq(secs.value<Hours>(), 2.1f / (60.0f * 60.0f)));
    static_assert(floatEq(secs.value<Minutes>(), 2.1f / 60.0f));
    static_assert(floatEq(secs.value<Seconds>(), 2.1f));
    static_assert(floatEq(secs.value<Milliseconds>(), 2100.0f));

	constexpr auto minutes = Value<Hours>{time};
    static_assert(floatEq(minutes.value<Hours>(), 2.1f / (60.0f * 60.0f)));
    static_assert(floatEq(minutes.value<Minutes>(), 2.1f / 60.0f));
    static_assert(floatEq(minutes.value<Seconds>(), 2.1f));
    static_assert(floatEq(minutes.value<Milliseconds>(), 2100.0f));

	constexpr auto hours = Value<Hours>{time};
    static_assert(floatEq(hours.value<Hours>(), 2.1f / (60.0f * 60.0f)));
    static_assert(floatEq(hours.value<Minutes>(), 2.1f / 60.0f));
    static_assert(floatEq(hours.value<Seconds>(), 2.1f));
    static_assert(floatEq(hours.value<Milliseconds>(), 2100.0f));

	constexpr auto millis = Value<Hours>{time};
    static_assert(floatEq(millis.value<Hours>(), 2.1f / (60.0f * 60.0f)));
    static_assert(floatEq(millis.value<Minutes>(), 2.1f / 60.0f));
    static_assert(floatEq(millis.value<Seconds>(), 2.1f));
    static_assert(floatEq(millis.value<Milliseconds>(), 2100.0f));
}

TEST(UnitTest, LiteralsCreation) {
	static_assert(floatEq((4.2_kg).value<Kilograms>(), 4.2f));
	static_assert(floatEq((4_kg).value<Kilograms>(), 4.0f));

	static_assert(floatEq((4.2_g).value<Grams>(), 4.2f));
	static_assert(floatEq((4_g).value<Grams>(), 4.0f));
}

TEST(UnitTest, ValueAddition) {
	constexpr auto sum = 3_kg + 2_g;
	static_assert(std::is_same_v<std::decay_t<decltype(sum)>, Value<Kilograms>>);
	static_assert(floatEq(sum.value<Kilograms>(), 3.002f));

	auto sumEq = 3_kg;
	sumEq += 2_g;
	EXPECT_FLOAT_EQ(sumEq.value<Kilograms>(), 3.002f);
}

TEST(UnitTest, ValueNegation) {
	static_assert(floatEq(-(3_kg).value<Kilograms>(), -3.0f));
	static_assert(floatEq((-3_kg).value<Kilograms>(), -3.0f));
	static_assert(floatEq(-(3.0_kg).value<Kilograms>(), -3.0f));
	static_assert(floatEq((-3.0_kg).value<Kilograms>(), -3.0f));
}

TEST(UnitTest, ValueSubtraction) {
	constexpr auto diff = 3_kg - 2_g;
	static_assert(std::is_same_v<std::decay_t<decltype(diff)>, Value<Kilograms>>);
	static_assert(floatEq(diff.value<Kilograms>(), 2.998f));

	auto diffEq = 3_kg;
	diffEq -= 2_g;
	EXPECT_FLOAT_EQ(diffEq.value<Kilograms>(), 2.998f);
}

TEST(UnitTest, ValueMultiplicationByUnitless) {
	constexpr auto sixKg = 3_kg * Value<Unitless>{2.0f};
	static_assert(std::is_same_v<std::decay_t<decltype(sixKg)>, Value<Kilograms>>);
	static_assert(floatEq(sixKg.value<Kilograms>(), 6.0f));

	auto threeTimesTwoKg = 3_kg;
	threeTimesTwoKg *= Value<Unitless>{2.0f};
	EXPECT_FLOAT_EQ(threeTimesTwoKg.value<Kilograms>(), sixKg.value<Kilograms>());
}

TEST(UnitTest, MassValueMultiplication) {
	constexpr auto threeKg = 3_kg;
	constexpr auto twoG = 2_g;
	constexpr auto threeG = 3_g;

	using GKg = decltype(Grams{} * Kilograms{});
	static_assert(GKg::RAD_EXP == 0);
	static_assert(std::ratio_equal_v<GKg::TO_M_RATIO, std::ratio<1, 1>>);
	static_assert(GKg::M_EXP == 0);
	static_assert(std::ratio_equal_v<GKg::TO_KG_RATIO, Grams::TO_KG_RATIO>);
	static_assert(GKg::KG_EXP == 2);
	static_assert(std::ratio_equal_v<GKg::TO_S_RATIO, std::ratio<1, 1>>);
	static_assert(GKg::S_EXP == 0);

	using GramsSq = decltype(Grams{} * Grams{});
	static_assert(GramsSq::RAD_EXP == 0);
	static_assert(std::ratio_equal_v<GramsSq::TO_M_RATIO, std::ratio<1, 1>>);
	static_assert(GramsSq::M_EXP == 0);
	static_assert(
		std::ratio_equal_v<
			GramsSq::TO_KG_RATIO,
			std::ratio_multiply<Grams::TO_KG_RATIO, Grams::TO_KG_RATIO>
		>
	);
	static_assert(GramsSq::KG_EXP == 2);
	static_assert(std::ratio_equal_v<GramsSq::TO_S_RATIO, std::ratio<1, 1>>);
	static_assert(GramsSq::S_EXP == 0);

	constexpr auto sixThousandGSq = threeKg * twoG;
	static_assert(std::is_same_v<std::decay_t<decltype(sixThousandGSq)>, Value<GKg>>);
	static_assert(floatEq(sixThousandGSq.value<GramsSq>(), 6000.0f));
	static_assert(floatEq(sixThousandGSq.value<GKg>(), 6.0f));
	static_assert(floatEq(sixThousandGSq.value<decltype(Kilograms{} * Kilograms{})>(), 0.006f));

	constexpr auto sixGSq = threeG * twoG;
	static_assert(std::is_same_v<std::decay_t<decltype(sixGSq)>, Value<GramsSq>>);
	static_assert(floatEq(sixGSq.value<GramsSq>(), 6.0f));
	static_assert(floatEq(sixGSq.value<decltype(Kilograms{} * Kilograms{})> (), 0.000006f));
}

TEST(UnitTest, ValueDivisionByUnitless) {
	constexpr auto twoKg = 4_kg / Value<Unitless>{2.0f};
	static_assert(std::is_same_v<std::decay_t<decltype(twoKg)>, Value<Kilograms>>);
	static_assert(floatEq(twoKg.value<Kilograms>(), 2.0f));

	auto fourDivTwoKg = 4_kg;
	fourDivTwoKg /= Value<Unitless>{2.0f};
	EXPECT_FLOAT_EQ(fourDivTwoKg.value<Kilograms>(), twoKg.value<Kilograms>());
}

TEST(UnitTest, MassValueDivision) {
	constexpr auto fourKg = 4_kg;
	constexpr auto twoG = 2_g;

	constexpr auto twoThousand = fourKg / twoG;
	static_assert(floatEq(twoThousand.value<Unitless>(), 2000.0f));
}

} // anonymous namespace
