#pragma once

#include "config.hpp"

namespace type_safety {

inline constexpr bool floatLT(float lhs, float rhs) {
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

template <int exp>
constexpr float pow([[maybe_unused]] float value) {
    if constexpr (exp == 0) {
        return 1.0f;
    } else if constexpr (exp > 0) {
        return value * pow<exp - 1>(value);
    } else {
        return (1.0f / value) * pow<exp + 1>(value);
    }
}

} // namespace type_safety
