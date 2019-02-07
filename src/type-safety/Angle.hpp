#pragma once

#include <utility>

#include "config.hpp"

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

class Angle final {
public:

    constexpr Angle() = default;

    constexpr Angle(RadiansTag, float r) :
        radians_(std::move(r))
    {
    }

    constexpr Angle(PiFactorTag, float piFactor) :
        radians_(piFactor * PI_)
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

    constexpr Angle& operator-=(const Angle& a) {
        radians_ -= a.radians_;
        return *this;
    }

    friend constexpr Angle operator-(Angle lhs, Angle rhs) {
        return Angle{std::move(lhs)} -= rhs;
    }

    friend constexpr Angle operator-(Angle a) {
        return Angle{radiansTag, -a.radians_};
    }

private:

    static constexpr auto PI_ = 3.141592f;
    static constexpr auto DEGREES_TO_RADIANS_ = PI_ / 180.0f;
    static constexpr auto RADIANS_TO_DEGREES_ = 180.0f / PI_;

    float radians_ = 0.0f;

};

constexpr auto PI = Angle{piFactorTag, 1.0f};

constexpr Angle operator""_deg(long double d) {
    return Angle{degreesTag, static_cast<float>(d)};
}

constexpr Angle operator""_rad(long double r) {
    return Angle{radiansTag, static_cast<float>(r)};
}

constexpr Angle operator""_pi(long double piFactor) {
    return Angle{piFactorTag, static_cast<float>(piFactor)};
}

} // namespace type_safety
