#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>
#include <sstream>

#include "type-safety/Angle.hpp"

using namespace type_safety;
using namespace type_safety::angle_literals;

namespace /* anonymous */ {

TEST(AngleTest, CreateFromDegrees) {
    constexpr auto degrees = 123.45f;
    constexpr auto radians = 2.154609f;
    constexpr auto angleFromConstructor = Angle{degreesTag, degrees};
    constexpr auto angleFromLiteral = 123.45_deg;
    constexpr auto integralAngleFromLiteral = 123_deg;

    static_assert(floatEq(angleFromConstructor.degrees(), degrees));
    static_assert(floatEq(angleFromConstructor.radians(), radians));
    static_assert(floatEq(angleFromLiteral.degrees(), degrees));
    static_assert(floatEq(integralAngleFromLiteral.degrees(), 123.0f));
}

TEST(AngleTest, CreateFromRadians) {
    constexpr auto radians = 2.154609f;
    constexpr auto degrees = 123.45f;
    constexpr auto angleFromConstructor = Angle{radiansTag, radians};
    constexpr auto angleFromLiteral = 2.154609_rad;
    constexpr auto integralAngleFromLiteral = 2_rad;

    static_assert(floatEq(angleFromConstructor.radians(), radians));
    static_assert(floatEq(angleFromConstructor.degrees(), degrees));
    static_assert(floatEq(angleFromLiteral.radians(), radians));
    static_assert(floatEq(integralAngleFromLiteral.radians(), 2.0f));
}

TEST(AngleTest, CreateFromPiFactor) {
    constexpr auto radians = PI * 0.5f;
    constexpr auto degrees = 90.0f;
    constexpr auto angleFromConstructor = Angle{piFactorTag, 0.5f};
    constexpr auto angleFromLiteral = 0.5_pi;
    constexpr auto integralAngleFromLiteral = 1_pi;

    static_assert(floatEq(angleFromConstructor.radians(), radians));
    static_assert(floatEq(angleFromConstructor.degrees(), degrees));
    static_assert(floatEq(angleFromLiteral.radians(), radians));
    static_assert(floatEq(integralAngleFromLiteral.radians(), 3.141592f));
}

TEST(AngleTest, AngleArithmetics) {
    static_assert(42.0_deg == 42.0_deg);
    static_assert(42.0_deg != 42.1_deg);
    
    static_assert(42.0_deg < 42.1_deg);
    static_assert(!(42.1_deg < 42.0_deg));
    static_assert(42.0_deg <= 42.1_deg);
    static_assert(42.0_deg <= 42.0_deg);
    static_assert(!(42.1_deg <= 42.0_deg));

    static_assert(42.1_deg > 42.0_deg);
    static_assert(!(42.0_deg > 42.1_deg));
    static_assert(42.1_deg >= 42.0_deg);
    static_assert(42.0_deg >= 42.0_deg);
    static_assert(!(42.0_deg >= 42.1_deg));

    static_assert(42.0_deg + 1.0_deg == 43.0_deg);
    static_assert(42.0_deg - 1.0_deg == 41.0_deg);
    static_assert(-42.0_deg == -(42.0_deg));
	static_assert(42_deg * 2.0f == 84_deg);
	static_assert(2.0f * 42_deg == 84_deg);
	static_assert(42_deg / 2.0f == 21_deg);

	auto plusEq = 42_deg;
	auto minusEq = 42_deg;
	auto timesEq = 42_deg;
	auto divEq = 42_deg;

	plusEq += 2_deg;
	minusEq -= 2_deg;
	timesEq *= 2.0f;
	divEq /= 2.0f;

	EXPECT_EQ(plusEq, 44_deg);
	EXPECT_EQ(minusEq, 40_deg);
	EXPECT_EQ(timesEq, 84_deg);
	EXPECT_EQ(divEq, 21_deg);
}

TEST(AngleTest, AnglePrintToOstream) {
    auto oss = std::ostringstream{};

    oss << -1_pi;

    EXPECT_EQ(oss.str(), "-180_deg");
}

} // anonymous namespace
