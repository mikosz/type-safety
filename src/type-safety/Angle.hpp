#pragma once

#include <utility>

namespace type_safety {

struct RadiansTag {
    constexpr RadiansTag() = default;
};
constexpr const auto radiansTag = RadiansTag{};

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

private:

    static constexpr auto PI_ = 3.141592f;
    static constexpr auto DEGREES_TO_RADIANS_ = PI_ / 180.0f;
    static constexpr auto RADIANS_TO_DEGREES_ = 180.0f / PI_;

    float radians_ = 0.0f;

};

constexpr Angle operator""_deg(long double d) {
    return Angle{degreesTag, static_cast<float>(d)};
}

constexpr Angle operator""_rad(long double r) {
    return Angle{radiansTag, static_cast<float>(r)};
}

} // namespace type_safety
