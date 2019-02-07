#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "type-safety/Angle.hpp"

using namespace type_safety;

namespace /* anonymous */ {

TEST(AngleTest, CreateFromDegrees) {
    constexpr auto degrees = 123.45f;
    constexpr auto radians = 2.154609f;
    constexpr auto angleFromConstructor = Angle{degreesTag, degrees};
    constexpr auto angleFromLiteral = 123.45_deg;

    EXPECT_FLOAT_EQ(angleFromConstructor.degrees(), degrees);
    EXPECT_FLOAT_EQ(angleFromConstructor.radians(), radians);
    EXPECT_FLOAT_EQ(angleFromLiteral.degrees(), degrees);
}

TEST(AngleTest, CreateFromRadians) {
    constexpr auto radians = 2.154609f;
    constexpr auto degrees = 123.45f;
    constexpr auto angleFromConstructor = Angle{radiansTag, radians};
    constexpr auto angleFromLiteral = 2.154609_rad;

    EXPECT_FLOAT_EQ(angleFromConstructor.radians(), radians);
    EXPECT_FLOAT_EQ(angleFromConstructor.degrees(), degrees);
    EXPECT_FLOAT_EQ(angleFromLiteral.radians(), radians);
}

} // anonymous namespace
