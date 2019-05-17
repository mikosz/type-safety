#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "type-safety/Unit.hpp"

using namespace type_safety;
using namespace type_safety::unit_literals;

namespace /* anonymous */ {

using Tonnes = MassUnit<std::kilo>;
using Pounds = MassUnit<std::ratio<45359, 100000>>;

TEST(UnitTest, DistanceCompatibleTypeConversion) {
    constexpr auto distance = makeValue<Metres>(2.1f);

	constexpr auto ms = Value<Metres>{distance};
    static_assert(floatEq(ms.value<Kilometres>(), 0.0021f));
    static_assert(floatEq(ms.value<Metres>(), 2.1f));

	constexpr auto kms = Value<Kilometres>{distance};
    static_assert(floatEq(kms.value<Kilometres>(), 0.0021f));
    static_assert(floatEq(kms.value<Metres>(), 2.1f));
}

TEST(UnitTest, MassCompatibleTypeConversion) {
    constexpr auto mass = makeValue<Kilograms>(2.1f);

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

	constexpr auto lbs = makeValue<Pounds>(2.2046f);
	static_assert(floatEq(lbs.value<Kilograms>(), 1.0f));
}

TEST(UnitTest, TimeCompatibleTypeConversion) {
    constexpr auto time = makeValue<Seconds>(2.1f);

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
	static_assert(floatEq((4.2_m).value<Metres>(), 4.2f));
	static_assert(floatEq((4_m).value<Metres>(), 4.0f));

	static_assert(floatEq((4.2_km).value<Kilometres>(), 4.2f));
	static_assert(floatEq((4_km).value<Kilometres>(), 4.0f));

	static_assert(floatEq((4.2_kg).value<Kilograms>(), 4.2f));
	static_assert(floatEq((4_kg).value<Kilograms>(), 4.0f));

	static_assert(floatEq((4.2_g).value<Grams>(), 4.2f));
	static_assert(floatEq((4_g).value<Grams>(), 4.0f));

	static_assert(floatEq((4.2_ms).value<Milliseconds>(), 4.2f));
	static_assert(floatEq((4_ms).value<Milliseconds>(), 4.0f));

	static_assert(floatEq((4.2_s).value<Seconds>(), 4.2f));
	static_assert(floatEq((4_s).value<Seconds>(), 4.0f));

	static_assert(floatEq((4.2_min).value<Minutes>(), 4.2f));
	static_assert(floatEq((4_min).value<Minutes>(), 4.0f));

	static_assert(floatEq((4.2_h).value<Hours>(), 4.2f));
	static_assert(floatEq((4_h).value<Hours>(), 4.0f));

	static_assert(floatEq((4.2_mps).value<MPS>(), 4.2f));
	static_assert(floatEq((4_mps).value<MPS>(), 4.0f));

	static_assert(floatEq((4.2_kph).value<KPH>(), 4.2f));
	static_assert(floatEq((4_kph).value<KPH>(), 4.0f));

	static_assert(floatEq((4.2_mps2).value<MPS2>(), 4.2f));
	static_assert(floatEq((4_mps2).value<MPS2>(), 4.0f));

	static_assert(floatEq((4.2_N).value<Newtons>(), 4.2f));
	static_assert(floatEq((4_N).value<Newtons>(), 4.0f));
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
	constexpr auto sixKg = 3_kg * 2.0f;
	static_assert(std::is_same_v<std::decay_t<decltype(sixKg)>, Value<Kilograms>>);
	static_assert(floatEq(sixKg.value<Kilograms>(), 6.0f));

	auto threeTimesTwoKg = 3_kg;
	threeTimesTwoKg *= 2.0f;
	EXPECT_FLOAT_EQ(threeTimesTwoKg.value<Kilograms>(), sixKg.value<Kilograms>());
}

TEST(UnitTest, MassValueMultiplication) {
	constexpr auto threeKg = 3_kg;
	constexpr auto twoG = 2_g;
	constexpr auto threeG = 3_g;

	using GKg = decltype(Grams{} * Kilograms{});
	static_assert(std::ratio_equal_v<GKg::TO_M_RATIO, std::ratio<1, 1>>);
	static_assert(GKg::M_EXP == 0);
	static_assert(std::ratio_equal_v<GKg::TO_KG_RATIO, Grams::TO_KG_RATIO>);
	static_assert(GKg::KG_EXP == 2);
	static_assert(std::ratio_equal_v<GKg::TO_S_RATIO, std::ratio<1, 1>>);
	static_assert(GKg::S_EXP == 0);

	using GramsSq = decltype(Grams{} * Grams{});
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
	constexpr auto twoKg = 4_kg / 2.0f;
	static_assert(std::is_same_v<std::decay_t<decltype(twoKg)>, Value<Kilograms>>);
	static_assert(floatEq(twoKg.value<Kilograms>(), 2.0f));

	auto fourDivTwoKg = 4_kg;
	fourDivTwoKg /= 2.0f;
	EXPECT_FLOAT_EQ(fourDivTwoKg.value<Kilograms>(), twoKg.value<Kilograms>());
}

TEST(UnitTest, MassValueDivision) {
	constexpr auto fourKg = 4_kg;
	constexpr auto twoG = 2_g;

	constexpr auto twoThousand = fourKg / twoG;
	static_assert(floatEq(twoThousand.value<Dimensionless>(), 2000.0f));
}

TEST(UnitTest, PrintsValuesWithUnitSuffix) {
	auto oss = std::ostringstream{};

	oss << 42_m << '\n'
		<< 42_km << '\n'
		<< 42_ms << '\n'
		<< 42_s << '\n'
		<< 42_min << '\n'
		<< 42_h << '\n'
		<< 42_mps << '\n'
		<< 42_kph << '\n'
		<< 42_mps2 << '\n'
		<< 42_N << '\n'
		;

	EXPECT_EQ(
		oss.str(),
		"42_m\n"
			"42_km\n"
			"42_ms\n"
			"42_s\n"
			"42_min\n"
			"42_h\n"
			"42_m/s\n"
			"42_km/h\n"
			"42_m/s2\n"
			"42_N\n"
	);
}

TEST(UnitTest, ComplexUnitConstruction) {
	constexpr auto force = 1_kg * 1_m / 1_s / 1_s;
	static_assert(std::is_same_v<std::decay_t<decltype(force)>, Force>);
	static_assert(floatEq(force.value<Newtons>(), (1_N).value<Newtons>()));
}

TEST(UnitTest, ValueComparison) {
	static_assert(2_N == 2_N);
	static_assert(2_N == 1_N + 1_N);
	static_assert(!(1_N == 2_N));

	static_assert(1_N != 2_N);
	static_assert(!(1_N != 1_N));

	static_assert(1_N < 2_N);
	static_assert(!(1_N < 1_N));
	static_assert(!(2_N < 1_N));

	static_assert(1_N <= 2_N);
	static_assert(2_N <= 2_N);
	static_assert(!(2_N <= 1_N));

	static_assert(3_N > 2_N);
	static_assert(!(3_N > 3_N));
	static_assert(!(2_N > 3_N));

	static_assert(3_N >= 2_N);
	static_assert(2_N >= 2_N);
	static_assert(!(2_N >= 3_N));
}

TEST(UnitTest, EarthGravitationalAcceleration) {
	constexpr auto R = 6.371_m; // * 10^6
	constexpr auto R_sq = R * R; // * 10^12
	constexpr auto M = 5.972_kg; // * 10^24
	using GUnit = decltype((Metres{} * Metres{} * Metres{}) / (Kilograms{} * Seconds{} * Seconds{}));
	constexpr auto G = makeValue<GUnit>(6.674f); // * 10^-11

	constexpr auto g = 10.0f * G * M / R_sq;

	static_assert((g - Acceleration{MPS2{}, 9.81f}).value<MPS2>() < 0.01f);
	static_assert((g - Acceleration{MPS2{}, 9.81f}).value<MPS2>() > -0.01f);
}

} // anonymous namespace
