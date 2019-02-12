#pragma once

#include <utility>
#include <iosfwd>

#include "math.hpp"

namespace type_safety {

struct RadiansTag {
    constexpr RadiansTag() = default;
};
constexpr const auto radiansTag = RadiansTag{};

struct PiFactorTag {
    constexpr PiFactorTag() = default;
};
constexpr const auto piFactorTag = PiFactorTag{};

struct DegreesTag {
    constexpr DegreesTag() = default;
};
constexpr const auto degreesTag = DegreesTag{};

constexpr auto PI = 3.141592f;

class Angle final {
public:

    constexpr Angle() = default;

    constexpr Angle(RadiansTag, float r) :
        radians_(std::move(r))
    {
    }

    constexpr Angle(PiFactorTag, float piFactor) :
        radians_(piFactor * PI)
    {
    }

    constexpr Angle(DegreesTag, float d) :
        radians_(d * DEGREES_TO_RADIANS_)
    {
    }

    constexpr float degrees() const {
        return radians_ * RADIANS_TO_DEGREES_;
    }

    constexpr float radians() const {
        return radians_;
    }

    friend constexpr bool operator==(Angle lhs, Angle rhs) {
        return floatEq(lhs.radians_, rhs.radians_);
    }

    friend constexpr bool operator!=(Angle lhs, Angle rhs) {
        return floatNE(lhs.radians_, rhs.radians_);
    }

    friend constexpr bool operator<(Angle lhs, Angle rhs) {
        return floatLT(lhs.radians_, rhs.radians_);
    }

    friend constexpr bool operator<=(Angle lhs, Angle rhs) {
        return floatLE(lhs.radians_, rhs.radians_);
    }

    friend constexpr bool operator>(Angle lhs, Angle rhs) {
        return floatGT(lhs.radians_, rhs.radians_);
    }

    friend constexpr bool operator>=(Angle lhs, Angle rhs) {
        return floatGE(lhs.radians_, rhs.radians_);
    }

    constexpr Angle& operator+=(Angle a) {
        radians_ += a.radians_;
        return *this;
    }

    friend constexpr Angle operator+(Angle lhs, Angle rhs) {
        return Angle{std::move(lhs)} += rhs;
    }

    constexpr Angle& operator-=(Angle a) {
        radians_ -= a.radians_;
        return *this;
    }

    friend constexpr Angle operator-(Angle lhs, Angle rhs) {
        return Angle{std::move(lhs)} -= rhs;
    }

    friend constexpr Angle operator-(Angle a) {
        return Angle{radiansTag, -a.radians_};
    }

    friend std::ostream& operator<<(std::ostream& os, Angle a) {
        return os << a.degrees() << "_deg";
    }

private:

    static constexpr auto DEGREES_TO_RADIANS_ = PI / 180.0f;
    static constexpr auto RADIANS_TO_DEGREES_ = 180.0f / PI;

    float radians_ = 0.0f;

};

namespace angle_literals {

inline constexpr Angle operator""_deg(long double d) {
    return Angle{degreesTag, static_cast<float>(d)};
}

inline constexpr Angle operator""_deg(unsigned long long d) {
    return Angle{degreesTag, static_cast<float>(d)};
}

inline constexpr Angle operator""_rad(long double r) {
    return Angle{radiansTag, static_cast<float>(r)};
}

inline constexpr Angle operator""_rad(unsigned long long r) {
    return Angle{radiansTag, static_cast<float>(r)};
}

inline constexpr Angle operator""_pi(long double piFactor) {
    return Angle{piFactorTag, static_cast<float>(piFactor)};
}

inline constexpr Angle operator""_pi(unsigned long long piFactor) {
    return Angle{piFactorTag, static_cast<float>(piFactor)};
}

} // namespace angle_literals

} // namespace type_safety
