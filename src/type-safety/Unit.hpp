#pragma once

#include "math.hpp"

namespace type_safety {

namespace detail {

template <
    int EXP_PARAM,
    int RAD_EXP_PARAM,
    int M_EXP_PARAM,
    int KG_EXP_PARAM,
    int S_EXP_PARAM
>
struct Unit final {

    static constexpr auto EXP = EXP_PARAM;
    static constexpr auto RAD_EXP = RAD_EXP_PARAM;
    static constexpr auto M_EXP = M_EXP_PARAM;
    static constexpr auto KG_EXP = KG_EXP_PARAM;
    static constexpr auto S_EXP = S_EXP_PARAM;

    template <class OtherUnitT>
    static constexpr auto IS_CONVERTIBLE_TO =
        RAD_EXP == OtherUnitT::RAD_EXP
        && M_EXP == OtherUnitT::M_EXP
        && KG_EXP == OtherUnitT::KG_EXP
        && S_EXP == OtherUnitT::S_EXP
        ;

    template <class OtherUnitT>
    static constexpr auto CONVERSION_FACTOR = pow<OtherUnitT::EXP - EXP>(10.0f);

};

} // namespace detail

template <int EXP>
using AngleUnit = detail::Unit<EXP, 1, 0, 0, 0>;

template <int EXP>
using DistanceUnit = detail::Unit<EXP, 0, 1, 0, 0>;

template <int EXP>
using MassUnit = detail::Unit<EXP, 0, 0, 1, 0>;

using Kilograms = MassUnit<0>;
using Grams = MassUnit<-3>;

template <int EXP>
using TimeUnit = detail::Unit<EXP, 0, 0, 0, 1>;

template <class UnitType>
class Value final {
public:

    using Unit = UnitType;

    constexpr Value() = default;

    explicit constexpr Value(float value) :
        value_(std::move(value))
    {
    }

    template <class CompatibleUnitT>
    constexpr Value(Value<CompatibleUnitT> compatibleValue) :
        value_(compatibleValue.value<Unit>())
    {
    }

    template <class CompatibleUnitT>
    constexpr float value() const {
        static_assert(Unit::IS_CONVERTIBLE_TO<CompatibleUnitT>);
        constexpr const auto FACTOR = CompatibleUnitT::CONVERSION_FACTOR<Unit>;
        if constexpr (FACTOR == 1) {
            return value_;
        } else {
			return static_cast<float>(FACTOR) * value_;
        }
    }

private:

    float value_;

};

namespace unit_literals {

inline constexpr Value<Kilograms> operator""_kg(long double value) {
	return Value<Kilograms>(static_cast<float>(value));
}

inline constexpr Value<Kilograms> operator""_kg(unsigned long long value) {
	return Value<Kilograms>(static_cast<float>(value));
}

inline constexpr Value<Grams> operator""_g(long double value) {
	return Value<Grams>(static_cast<float>(value));
}

inline constexpr Value<Grams> operator""_g(unsigned long long value) {
	return Value<Grams>(static_cast<float>(value));
}

} // namespace unit_literals

} // namespace type_safety
