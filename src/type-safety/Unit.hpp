#pragma once

#include <ratio>
#include <iostream>

#include "math.hpp"

namespace type_safety {

namespace detail {

template <
	class TO_M_RATIO_PARAM,
    int M_EXP_PARAM,
    class TO_KG_RATIO_PARAM,
	int KG_EXP_PARAM,
	class TO_S_RATIO_PARAM,
	int S_EXP_PARAM
>
struct Unit final {

	using TO_M_RATIO = TO_M_RATIO_PARAM;
    static constexpr auto M_EXP = M_EXP_PARAM;
	using TO_KG_RATIO = TO_KG_RATIO_PARAM;
    static constexpr auto KG_EXP = KG_EXP_PARAM;
	using TO_S_RATIO = TO_S_RATIO_PARAM;
    static constexpr auto S_EXP = S_EXP_PARAM;

    template <class OtherUnitT>
	static constexpr auto IS_CONVERTIBLE_TO =
		M_EXP == OtherUnitT::M_EXP
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
			std::ratio_divide<TO_M_RATIO, OtherUnitT::TO_M_RATIO>,
			M_EXP - OtherUnitT::M_EXP,
			std::ratio_divide<TO_KG_RATIO, OtherUnitT::TO_KG_RATIO>,
			KG_EXP - OtherUnitT::KG_EXP,
			std::ratio_divide<TO_S_RATIO, OtherUnitT::TO_S_RATIO>,
			S_EXP - OtherUnitT::S_EXP
		>{};
	}

	template <class CompatibleUnitT>
	constexpr float convertTo(float value) const {
		static_assert(Unit::IS_CONVERTIBLE_TO<CompatibleUnitT>);
		using Ratio = Unit::ConversionRatio<CompatibleUnitT>;
		if constexpr (std::ratio_equal_v<Ratio, std::ratio<1, 1>>) {
			return value;
		} else {
			return (static_cast<float>(Ratio::num) / static_cast<float>(Ratio::den)) * value;
		}
	}

};

} // namespace detail

using Dimensionless = detail::Unit<std::ratio<1>, 0, std::ratio<1>, 0, std::ratio<1>, 0>;

inline std::ostream& operator<<(std::ostream& os, Dimensionless) {
	return os;
}

template <class ToMRatio>
using DistanceUnit = detail::Unit<ToMRatio, 1, std::ratio<1>, 0, std::ratio<1>, 0>;

template <class ToKgRatio>
using MassUnit = detail::Unit<std::ratio<1>, 0, ToKgRatio, 1, std::ratio<1>, 0>;

template <class ToSRatio >
using TimeUnit = detail::Unit<std::ratio<1>, 0, std::ratio<1>, 0, ToSRatio, 1>;

using Metres = DistanceUnit<std::ratio<1>>;
using Kilometres = DistanceUnit<std::kilo>;

inline std::ostream& operator<<(std::ostream& os, Metres) {
	return os << "_m";
}

inline std::ostream& operator<<(std::ostream& os, Kilometres) {
	return os << "_km";
}

using Kilograms = MassUnit<std::ratio<1>>;
using Grams = MassUnit<std::milli>;

inline std::ostream& operator<<(std::ostream& os, Kilograms) {
	return os << "_kg";
}

inline std::ostream& operator<<(std::ostream& os, Grams) {
	return os << "_g";
}

using Milliseconds = TimeUnit<std::milli>;
using Seconds = TimeUnit<std::ratio<1>>;
using Minutes = TimeUnit<std::ratio<60>>;
using Hours = TimeUnit<std::ratio<60 * 60>>;

inline std::ostream& operator<<(std::ostream& os, Milliseconds) {
	return os << "_ms";
}

inline std::ostream& operator<<(std::ostream& os, Seconds) {
	return os << "_s";
}

inline std::ostream& operator<<(std::ostream& os, Minutes) {
	return os << "_min";
}

inline std::ostream& operator<<(std::ostream& os, Hours) {
	return os << "_h";
}

using MPS = decltype(Metres{} / Seconds{});
using KPH = decltype(Kilometres{} / Hours{});
using MPS2 = decltype(MPS{} / Seconds{});
using Newtons = decltype(Kilograms{} * MPS2{});

inline std::ostream& operator<<(std::ostream& os, MPS) {
	return os << "_m/s";
}

inline std::ostream& operator<<(std::ostream& os, KPH) {
	return os << "_km/h";
}

inline std::ostream& operator<<(std::ostream& os, MPS2) {
	return os << "_m/s2";
}

inline std::ostream& operator<<(std::ostream& os, Newtons) {
	return os << "_N";
}

template <class UnitType>
class Value final {
public:

    using Unit = UnitType;

    constexpr Value() = default;

	template <class CompatibleUnitT>
    explicit constexpr Value(CompatibleUnitT unit, float value) :
        value_(unit.convertTo<Unit>(value))
    {
    }

    template <class CompatibleUnitT>
    constexpr Value(Value<CompatibleUnitT> compatibleValue) :
        value_(compatibleValue.value<Unit>())
    {
    }

    template <class CompatibleUnitT>
    constexpr float value() const {
		return Unit{}.convertTo<CompatibleUnitT>(value_);
    }

	template <class CompatibleUnitT>
	friend constexpr bool operator==(Value lhs, Value<CompatibleUnitT> rhs) {
		return floatEq(lhs.value_, rhs.value<Unit>());
	}

	template <class CompatibleUnitT>
	friend constexpr bool operator!=(Value lhs, Value<CompatibleUnitT> rhs) {
		return floatNE(lhs.value_, rhs.value<Unit>());
	}

	template <class CompatibleUnitT>
	friend constexpr bool operator<(Value lhs, Value<CompatibleUnitT> rhs) {
		return floatLT(lhs.value_, rhs.value<Unit>());
	}

	template <class CompatibleUnitT>
	friend constexpr bool operator<=(Value lhs, Value<CompatibleUnitT> rhs) {
		return floatLE(lhs.value_, rhs.value<Unit>());
	}

	template <class CompatibleUnitT>
	friend constexpr bool operator>(Value lhs, Value<CompatibleUnitT> rhs) {
		return floatGT(lhs.value_, rhs.value<Unit>());
	}

	template <class CompatibleUnitT>
	friend constexpr bool operator>=(Value lhs, Value<CompatibleUnitT> rhs) {
		return floatGE(lhs.value_, rhs.value<Unit>());
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
		return Value{Unit{}, -value_};
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

	constexpr Value& operator*=(float scalar) {
		value_ *= scalar;
		return *this;
	}

	template <class OtherUnit>
	friend constexpr auto operator*(Value lhs, Value<OtherUnit> rhs) {
		using UnitProduct = decltype(decltype(lhs)::Unit{} * decltype(rhs)::Unit{});
		return Value<UnitProduct>{UnitProduct{}, lhs.value_ * rhs.value<OtherUnit>()};
	}

	friend constexpr Value operator*(Value v, float scalar) {
		return Value{Unit{}, v.value_ * scalar};
	}

	friend constexpr Value operator*(float scalar, Value v) {
		return Value{Unit{}, v.value_ * scalar};
	}

	constexpr Value& operator/=(float scalar) {
		value_ /= scalar;
		return *this;
	}

	template <class OtherUnit>
	friend constexpr auto operator/(Value lhs, Value<OtherUnit> rhs) {
		using UnitQuotient = decltype(decltype(lhs)::Unit{} / decltype(rhs)::Unit{});
		return Value<UnitQuotient>{UnitQuotient{}, lhs.value_ / rhs.value<OtherUnit>()};
	}

	friend constexpr Value operator/(Value v, float scalar) {
		return Value{Unit{}, v.value_ / scalar};
	}

	friend constexpr Value operator/(float scalar, Value v) {
		return Value{Unit{}, v.value_ / scalar};
	}

	friend inline std::ostream& operator<<(std::ostream& os, Value value) {
		return os << value.value_ << Unit{};
	}

private:

    float value_;

};

template <class UnitT>
inline constexpr Value<UnitT> makeValue(float value) {
	return Value<UnitT>(UnitT{}, value);
}

using Distance = Value<Metres>;
using Mass = Value<Kilograms>;
using Time = Value<Seconds>;
using Speed = Value<MPS>;
// #TODO: acceleration and force are vectors, so it may not be the best idea to reserve
// these names.
using Acceleration = Value<MPS2>;
using Force = Value<Newtons>;

static_assert(sizeof(Distance) == sizeof(float));

namespace unit_literals {

inline constexpr Value<Metres> operator""_m(long double value) {
	return Value<Metres>{Metres{}, static_cast<float>(value)};
}

inline constexpr Value<Metres> operator""_m(unsigned long long value) {
	return Value<Metres>{Metres{}, static_cast<float>(value)};
}

inline constexpr Value<Metres> operator""_km(long double value) {
	return Value<Kilometres>{Kilometres{}, static_cast<float>(value)};
}

inline constexpr Value<Kilometres> operator""_km(unsigned long long value) {
	return Value<Kilometres>{Kilometres{}, static_cast<float>(value)};
}

inline constexpr Value<Kilograms> operator""_kg(long double value) {
	return Value<Kilograms>{Kilograms{}, static_cast<float>(value)};
}

inline constexpr Value<Kilograms> operator""_kg(unsigned long long value) {
	return Value<Kilograms>{Kilograms{}, static_cast<float>(value)};
}

inline constexpr Value<Grams> operator""_g(long double value) {
	return Value<Grams>{Grams{}, static_cast<float>(value)};
}

inline constexpr Value<Grams> operator""_g(unsigned long long value) {
	return Value<Grams>{Grams{}, static_cast<float>(value)};
}

inline constexpr Value<Milliseconds> operator""_ms(long double value) {
	return Value<Milliseconds>{Milliseconds{}, static_cast<float>(value)};
}

inline constexpr Value<Milliseconds> operator""_ms(unsigned long long value) {
	return Value<Milliseconds>{Milliseconds{}, static_cast<float>(value)};
}

inline constexpr Value<Seconds> operator""_s(long double value) {
	return Value<Seconds>{Seconds{}, static_cast<float>(value)};
}

inline constexpr Value<Seconds> operator""_s(unsigned long long value) {
	return Value<Seconds>{Seconds{}, static_cast<float>(value)};
}

inline constexpr Value<Minutes> operator""_min(long double value) {
	return Value<Minutes>{Minutes{}, static_cast<float>(value)};
}

inline constexpr Value<Minutes> operator""_min(unsigned long long value) {
	return Value<Minutes>{Minutes{}, static_cast<float>(value)};
}

inline constexpr Value<Hours> operator""_h(long double value) {
	return Value<Hours>{Hours{}, static_cast<float>(value)};
}

inline constexpr Value<Hours> operator""_h(unsigned long long value) {
	return Value<Hours>{Hours{}, static_cast<float>(value)};
}

inline constexpr Value<MPS> operator""_mps(long double value) {
	return Value<MPS>{MPS{}, static_cast<float>(value)};
}

inline constexpr Value<MPS> operator""_mps(unsigned long long value) {
	return Value<MPS>{MPS{}, static_cast<float>(value)};
}

inline constexpr Value<KPH> operator""_kph(long double value) {
	return Value<KPH>{KPH{}, static_cast<float>(value)};
}

inline constexpr Value<KPH> operator""_kph(unsigned long long value) {
	return Value<KPH>{KPH{}, static_cast<float>(value)};
}

inline constexpr Value<MPS2> operator""_mps2(long double value) {
	return Value<MPS2>{MPS2{}, static_cast<float>(value)};
}

inline constexpr Value<MPS2> operator""_mps2(unsigned long long value) {
	return Value<MPS2>{MPS2{}, static_cast<float>(value)};
}

inline constexpr Value<Newtons> operator""_N(long double value) {
	return Value<Newtons>{Newtons{}, static_cast<float>(value)};
}

inline constexpr Value<Newtons> operator""_N(unsigned long long value) {
	return Value<Newtons>{Newtons{}, static_cast<float>(value)};
}

} // namespace unit_literals

} // namespace type_safety
