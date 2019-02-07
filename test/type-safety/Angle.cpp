#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "type-safety/Angle.hpp"

using namespace type_safety;
using namespace type_safety::angle_literals;

namespace /* anonymous */ {

TEST(AngleTest, CreateFromDegrees) {
    constexpr auto degrees = 123.45f;
    constexpr auto radians = 2.154609f;
    constexpr auto angleFromConstructor = Angle{degreesTag, degrees};
    constexpr auto angleFromLiteral = 123.45_deg;

    static_assert(floatEq(angleFromConstructor.degrees(), degrees));
    static_assert(floatEq(angleFromConstructor.radians(), radians));
    static_assert(floatEq(angleFromLiteral.degrees(), degrees));
}

TEST(AngleTest, CreateFromRadians) {
    constexpr auto radians = 2.154609f;
    constexpr auto degrees = 123.45f;
    constexpr auto angleFromConstructor = Angle{radiansTag, radians};
    constexpr auto angleFromLiteral = 2.154609_rad;

    static_assert(floatEq(angleFromConstructor.radians(), radians));
    static_assert(floatEq(angleFromConstructor.degrees(), degrees));
    static_assert(floatEq(angleFromLiteral.radians(), radians));
}

TEST(AngleTest, CreateFromPiFactor) {
    constexpr auto radians = 3.141592f * 0.5f;
    constexpr auto degrees = 90.0f;
    constexpr auto angleFromConstructor = Angle{piFactorTag, 0.5f};
    constexpr auto angleFromLiteral = 0.5_pi;

    static_assert(floatEq(angleFromConstructor.radians(), radians));
    static_assert(floatEq(angleFromConstructor.degrees(), degrees));
    static_assert(floatEq(angleFromLiteral.radians(), radians));
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

} // anonymous namespace
