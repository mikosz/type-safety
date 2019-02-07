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
    constexpr auto radians = 3.141592f * 0.5f;
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
}

TEST(AngleTest, AnglePrintToOstream) {
    auto oss = std::ostringstream{};

    oss << -1_pi;

    EXPECT_EQ(oss.str(), "-180_deg");
}

} // anonymous namespace
