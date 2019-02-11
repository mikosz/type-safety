#pragma once

namespace type_safety {

#ifdef _FLOAT_EQ_EPSILON
constexpr const auto FLOAT_EQ_EPSILON = _FLOAT_EQ_EPSILON;
#else
constexpr const auto FLOAT_EQ_EPSILON = 0.0001f;
#endif /* _FLOAT_EQ_EPSILON */

} // namespace type_safety
