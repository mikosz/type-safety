#pragma once

#include <ratio>

#include "math.hpp"

namespace type_safety {

namespace detail {

template <
    int RAD_EXP_PARAM,
	int TO_METRES_RATIO_NUM_PARAM,
	int TO_METRES_RATIO_DEN_PARAM,
    int M_EXP_PARAM,
    int TO_KILOGRAMS_RATIO_NUM_PARAM,
    int TO_KILOGRAMS_RATIO_DEN_PARAM,
	int KG_EXP_PARAM,
	int TO_SECONDS_RATIO_NUM_PARAM,
	int TO_SECONDS_RATIO_DEN_PARAM,
	int S_EXP_PARAM
>
struct Unit final {

    static constexpr auto RAD_EXP = RAD_EXP_PARAM;
	using TO_M_RATIO = std::ratio<TO_METRES_RATIO_NUM_PARAM, TO_METRES_RATIO_DEN_PARAM>;
    static constexpr auto M_EXP = M_EXP_PARAM;
	using TO_KG_RATIO = std::ratio<TO_KILOGRAMS_RATIO_NUM_PARAM, TO_KILOGRAMS_RATIO_DEN_PARAM>;
    static constexpr auto KG_EXP = KG_EXP_PARAM;
	using TO_S_RATIO = std::ratio<TO_SECONDS_RATIO_NUM_PARAM, TO_SECONDS_RATIO_DEN_PARAM>;
    static constexpr auto S_EXP = S_EXP_PARAM;

    template <class OtherUnitT>
	static constexpr auto IS_CONVERTIBLE_TO =
		RAD_EXP == OtherUnitT::RAD_EXP
		&& M_EXP == OtherUnitT::M_EXP
		&& KG_EXP == OtherUnitT::KG_EXP
        && S_EXP == OtherUnitT::S_EXP
        ;

    template <class OtherUnitT>
    using ConversionRatio =
		std::ratio_multiply<
			std::ratio_divide<TO_M_RATIO, typename OtherUnitT::TO_M_RATIO>,
			std::ratio_multiply<
				std::ratio_divide<TO_KG_RATIO, typename OtherUnitT::TO_KG_RATIO>,
				std::ratio_divide<TO_S_RATIO, typename OtherUnitT::TO_S_RATIO>
			>
		>
		;

	constexpr Unit() = default;

	template <class OtherUnitT>
	constexpr auto operator*(OtherUnitT) const {
		return Unit<
			RAD_EXP + OtherUnitT::RAD_EXP,
			TO_M_RATIO::num * OtherUnitT::TO_M_RATIO::num,
			TO_M_RATIO::den * OtherUnitT::TO_M_RATIO::den,
			M_EXP + OtherUnitT::M_EXP,
			TO_KG_RATIO::num * OtherUnitT::TO_KG_RATIO::num,
			TO_KG_RATIO::den * OtherUnitT::TO_KG_RATIO::den,
			KG_EXP + OtherUnitT::KG_EXP,
			TO_S_RATIO::num * OtherUnitT::TO_S_RATIO::num,
			TO_S_RATIO::den * OtherUnitT::TO_S_RATIO::den,
			S_EXP + OtherUnitT::S_EXP
		>{};
	}

	template <class OtherUnitT>
	constexpr auto operator/(OtherUnitT) const {
		return Unit<
			RAD_EXP - OtherUnitT::RAD_EXP,
			TO_M_RATIO::num * OtherUnitT::TO_M_RATIO::den,
			TO_M_RATIO::den * OtherUnitT::TO_M_RATIO::num,
			M_EXP - OtherUnitT::M_EXP,
			TO_KG_RATIO::num * OtherUnitT::TO_KG_RATIO::den,
			TO_KG_RATIO::den * OtherUnitT::TO_KG_RATIO::num,
			KG_EXP - OtherUnitT::KG_EXP,
			TO_S_RATIO::num * OtherUnitT::TO_S_RATIO::den,
			TO_S_RATIO::den * OtherUnitT::TO_S_RATIO::num,
			S_EXP - OtherUnitT::S_EXP
		>{};
	}

};

} // namespace detail

using Unitless = detail::Unit<0, 1, 1, 0, 1, 1, 0, 1, 1, 0>;

template <int NUM, int DEN>
using DistanceUnit = detail::Unit<0, NUM, DEN, 1, 1, 1, 0, 1, 1, 0>;

template <int NUM, int DEN>
using MassUnit = detail::Unit<0, 1, 1, 0, NUM, DEN, 1, 1, 1, 0>;

template <int NUM, int DEN>
using TimeUnit = detail::Unit<0, 1, 1, 0, 1, 1, 0, NUM, DEN, 1>;

using Metres = DistanceUnit<1, 1>;
using Kilometres = DistanceUnit<1000, 1>;

using Kilograms = MassUnit<1, 1>;
using Grams = MassUnit<1, 1000>;

using Milliseconds = TimeUnit<1, 1000>;
using Seconds = TimeUnit<1, 1>;
using Minutes = TimeUnit<60, 1>;
using Hours = TimeUnit<60 * 60, 1>;

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
        using Ratio = Unit::ConversionRatio<CompatibleUnitT>;
        if constexpr (std::ratio_equal_v<Ratio, std::ratio<1, 1>>) {
            return value_;
        } else {
			return (static_cast<float>(Ratio::num) / static_cast<float>(Ratio::den)) * value_;
        }
    }

	template <class CompatibleUnitT>
	constexpr Value& operator+=(Value<CompatibleUnitT> other) {
		value_ += other.value<Unit>();
		return *this;
	}

	template <class CompatibleUnitT>
	friend constexpr Value operator+(Value lhs, Value<CompatibleUnitT> rhs) {
		lhs += rhs;
		return lhs;
	}

	constexpr Value operator-() const {
		return Value{-value_};
	}

	template <class CompatibleUnitT>
	constexpr Value& operator-=(Value<CompatibleUnitT> other) {
		value_ -= other.value<Unit>();
		return *this;
	}

	template <class CompatibleUnitT>
	friend constexpr Value operator-(Value lhs, Value<CompatibleUnitT> rhs) {
		lhs -= rhs;
		return lhs;
	}

	constexpr Value& operator*=(Value<Unitless> unitless) {
		value_ *= unitless.value<Unitless>();
		return *this;
	}

	template <class OtherUnit>
	friend constexpr auto operator*(Value lhs, Value<OtherUnit> rhs) {
		using UnitProduct = decltype(decltype(lhs)::Unit{} * decltype(rhs)::Unit{});
		return Value<UnitProduct>{lhs.value_ * rhs.value<OtherUnit>()};
	}

	constexpr Value& operator/=(Value<Unitless> unitless) {
		value_ /= unitless.value<Unitless>();
		return *this;
	}

	template <class OtherUnit>
	friend constexpr auto operator/(Value lhs, Value<OtherUnit> rhs) {
		using UnitQuotient = decltype(decltype(lhs)::Unit{} / decltype(rhs)::Unit{});
		return Value<UnitQuotient>{lhs.value_ / rhs.value<OtherUnit>()};
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
