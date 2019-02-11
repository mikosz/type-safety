#pragma once

#include <ratio>

#include "math.hpp"

namespace type_safety {

namespace detail {

template <
    int RAD_EXP_PARAM,
    int M_EXP_PARAM,
    int TO_KILOGRAMS_RATIO_NUM_PARAM,
    int TO_KILOGRAMS_RATIO_DEN_PARAM,
	int KG_EXP_PARAM,
    int S_EXP_PARAM
>
struct Unit final {

    static constexpr auto RAD_EXP = RAD_EXP_PARAM;
    static constexpr auto M_EXP = M_EXP_PARAM;
	using TO_KG_RATIO = std::ratio<TO_KILOGRAMS_RATIO_NUM_PARAM, TO_KILOGRAMS_RATIO_DEN_PARAM>;
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
    using CONVERSION_RATIO =
		std::ratio_divide<typename OtherUnitT::TO_KG_RATIO, TO_KG_RATIO>
		;

};

} // namespace detail

template <int NUM, int DEN>
using MassUnit = detail::Unit<0, 0, NUM, DEN, 1, 0>;

using Kilograms = MassUnit<1, 1>;
using Grams = MassUnit<1000, 1>;

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
        using RATIO = Unit::CONVERSION_RATIO<CompatibleUnitT>;
        if constexpr (std::ratio_equal_v<RATIO, std::ratio<1, 1>>) {
            return value_;
        } else {
			return (static_cast<float>(RATIO::num) / static_cast<float>(RATIO::den)) * value_;
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
