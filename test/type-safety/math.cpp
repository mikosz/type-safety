#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "type-safety/math.hpp"

using namespace type_safety;

namespace /* anonymous */ {

TEST(MathTest, FloatComparison) {
    static_assert(floatEq(0.0001f, 0.0001f));
    static_assert(floatEq(0.00001f, 0.00002f));

    static_assert(floatLT(0.0001f, 0.0003f));
    static_assert(!floatLT(0.00001f, 0.00002f));
    static_assert(!floatLT(0.0001f, 0.0001f));
    static_assert(!floatLT(0.0002f, 0.0001f));

    static_assert(floatLE(0.0001f, 0.0003f));
    static_assert(floatLE(0.00001f, 0.00002f));
    static_assert(floatLE(0.0001f, 0.0001f));
    static_assert(!floatLE(0.0003f, 0.0001f));

    static_assert(floatGT(0.0003f, 0.0001f));
    static_assert(!floatGT(0.00002f, 0.00001f));
    static_assert(!floatGT(0.0001f, 0.0001f));
    static_assert(!floatGT(0.0001f, 0.0002f));

    static_assert(floatGE(0.0003f, 0.0001f));
    static_assert(floatGE(0.00002f, 0.00001f));
    static_assert(floatGE(0.0001f, 0.0001f));
    static_assert(!floatGE(0.0001f, 0.0003f));
}

TEST(MathTest, PowCalculatesPower) {
    static_assert(floatEq(pow<-2>(10.0f), 0.01f));
    static_assert(floatEq(pow<-1>(10.0f), 0.1f));
    static_assert(floatEq(pow<0>(10.0f), 1.0f));
    static_assert(floatEq(pow<1>(10.0f), 10.0f));
    static_assert(floatEq(pow<2>(10.0f), 100.0f));
}

} // anonymous namespace
