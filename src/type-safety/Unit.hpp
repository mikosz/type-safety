#pragma once

#include <ratio>

#include "math.hpp"

namespace type_safety {

namespace detail {

template <
    int RAD_EXP_PARAM,
	class TO_M_RATIO_PARAM,
    int M_EXP_PARAM,
    class TO_KG_RATIO_PARAM,
	int KG_EXP_PARAM,
	class TO_S_RATIO_PARAM,
	int S_EXP_PARAM
>
struct Unit final {

    static constexpr auto RAD_EXP = RAD_EXP_PARAM;
	using TO_M_RATIO = TO_M_RATIO_PARAM;
    static constexpr auto M_EXP = M_EXP_PARAM;
	using TO_KG_RATIO = TO_KG_RATIO_PARAM;
    static constexpr auto KG_EXP = KG_EXP_PARAM;
	using TO_S_RATIO = TO_S_RATIO_PARAM;
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
			std::ratio_multiply<TO_M_RATIO, OtherUnitT::TO_M_RATIO>,
			M_EXP + OtherUnitT::M_EXP,
			std::ratio_multiply<TO_KG_RATIO, OtherUnitT::TO_KG_RATIO>,
			KG_EXP + OtherUnitT::KG_EXP,
			std::ratio_multiply<TO_S_RATIO, OtherUnitT::TO_S_RATIO>,
			S_EXP + OtherUnitT::S_EXP
		>{};
	}

	template <class OtherUnitT>
	constexpr auto operator/(OtherUnitT) const {
		return Unit<
			RAD_EXP - OtherUnitT::RAD_EXP,
			std::ratio_divide<TO_M_RATIO, OtherUnitT::TO_M_RATIO>,
			M_EXP - OtherUnitT::M_EXP,
			std::ratio_divide<TO_KG_RATIO, OtherUnitT::TO_KG_RATIO>,
			KG_EXP - OtherUnitT::KG_EXP,
			std::ratio_divide<TO_S_RATIO, OtherUnitT::TO_S_RATIO>,
			S_EXP - OtherUnitT::S_EXP
		>{};
	}

};

} // namespace detail

using RatioOne = std::ratio<1>;

using Unitless = detail::Unit<0, RatioOne, 0, RatioOne, 0, RatioOne, 0>;

template <class ToKgRatio>
using DistanceUnit = detail::Unit<0, ToKgRatio, 1, RatioOne, 0, RatioOne, 0>;

template <class ToKgRatio>
using MassUnit = detail::Unit<0, RatioOne, 0, ToKgRatio, 1, RatioOne, 0>;

template <class ToSRatio >
using TimeUnit = detail::Unit<0, RatioOne, 0, RatioOne, 0, ToSRatio, 1>;

using Metres = DistanceUnit<RatioOne>;
using Kilometres = DistanceUnit<std::kilo>;

using Kilograms = MassUnit<RatioOne>;
using Grams = MassUnit<std::milli>;

using Milliseconds = TimeUnit<std::milli>;
using Seconds = TimeUnit<RatioOne>;
using Minutes = TimeUnit<std::ratio<60>>;
using Hours = TimeUnit<std::ratio<60 * 60>>;

using MPS = decltype(Metres{} / Seconds{});
using KPH = decltype(Kilometres{} / Hours{});
using MPS2 = decltype(MPS{} / Seconds{});
using Newtons = decltype(Kilograms{} * MPS2{});

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

using Distance = Value<Metres>;
using Mass = Value<Kilograms>;
using Time = Value<Seconds>;
// #TODO: velocity, acceleration and force are all vectors, so it may not be the best idea to reserve
// these names. Velocity could be renamed to Speed, but what about Acceleration and Force?
using Velocity = Value<MPS>;
using Acceleration = Value<MPS2>;
using Force = Value<Newtons>;

namespace unit_literals {

inline constexpr Value<Metres> operator""_m(long double value) {
	return Value<Metres>{static_cast<float>(value)};
}

inline constexpr Value<Metres> operator""_m(unsigned long long value) {
	return Value<Metres>{static_cast<float>(value)};
}

inline constexpr Value<Metres> operator""_km(long double value) {
	return Value<Kilometres>{static_cast<float>(value)};
}

inline constexpr Value<Kilometres> operator""_km(unsigned long long value) {
	return Value<Kilometres>{static_cast<float>(value)};
}

inline constexpr Value<Kilograms> operator""_kg(long double value) {
	return Value<Kilograms>{static_cast<float>(value)};
}

inline constexpr Value<Kilograms> operator""_kg(unsigned long long value) {
	return Value<Kilograms>{static_cast<float>(value)};
}

inline constexpr Value<Grams> operator""_g(long double value) {
	return Value<Grams>{static_cast<float>(value)};
}

inline constexpr Value<Grams> operator""_g(unsigned long long value) {
	return Value<Grams>{static_cast<float>(value)};
}

inline constexpr Value<Milliseconds> operator""_ms(long double value) {
	return Value<Milliseconds>{static_cast<float>(value)};
}

inline constexpr Value<Milliseconds> operator""_ms(unsigned long long value) {
	return Value<Milliseconds>{static_cast<float>(value)};
}

inline constexpr Value<Seconds> operator""_s(long double value) {
	return Value<Seconds>{static_cast<float>(value)};
}

inline constexpr Value<Seconds> operator""_s(unsigned long long value) {
	return Value<Seconds>{static_cast<float>(value)};
}

inline constexpr Value<Minutes> operator""_min(long double value) {
	return Value<Minutes>{static_cast<float>(value)};
}

inline constexpr Value<Minutes> operator""_min(unsigned long long value) {
	return Value<Minutes>{static_cast<float>(value)};
}

inline constexpr Value<Hours> operator""_h(long double value) {
	return Value<Hours>{static_cast<float>(value)};
}

inline constexpr Value<Hours> operator""_h(unsigned long long value) {
	return Value<Hours>{static_cast<float>(value)};
}

inline constexpr Value<MPS> operator""_mps(long double value) {
	return Value<MPS>{static_cast<float>(value)};
}

inline constexpr Value<MPS> operator""_mps(unsigned long long value) {
	return Value<MPS>{static_cast<float>(value)};
}

inline constexpr Value<KPH> operator""_kph(long double value) {
	return Value<KPH>{static_cast<float>(value)};
}

inline constexpr Value<KPH> operator""_kph(unsigned long long value) {
	return Value<KPH>{static_cast<float>(value)};
}

inline constexpr Value<MPS2> operator""_mps2(long double value) {
	return Value<MPS2>{static_cast<float>(value)};
}

inline constexpr Value<MPS2> operator""_mps2(unsigned long long value) {
	return Value<MPS2>{static_cast<float>(value)};
}

inline constexpr Value<Newtons> operator""_N(long double value) {
	return Value<Newtons>{static_cast<float>(value)};
}

inline constexpr Value<Newtons> operator""_N(unsigned long long value) {
	return Value<Newtons>{static_cast<float>(value)};
}

} // namespace unit_literals

} // namespace type_safety
