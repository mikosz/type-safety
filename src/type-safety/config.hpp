#pragma once

namespace type_safety {

constexpr const auto FLOAT_EQ_EPSILON = 0.0001f;

constexpr bool floatLT(float lhs, float rhs) {
    return lhs < rhs - FLOAT_EQ_EPSILON;
}

constexpr bool floatLE(float lhs, float rhs) {
    return lhs <= rhs + FLOAT_EQ_EPSILON;
}

constexpr bool floatGT(float lhs, float rhs) {
    return lhs > rhs + FLOAT_EQ_EPSILON;
}

constexpr bool floatGE(float lhs, float rhs) {
    return lhs >= rhs - FLOAT_EQ_EPSILON;
}

constexpr bool floatEq(float lhs, float rhs) {
    return floatLE(lhs, rhs) && floatGE(lhs, rhs);
}

constexpr bool floatNE(float lhs, float rhs) {
    return floatLT(lhs, rhs) || floatLT(rhs, lhs);
}

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

} // namespace type_safety
