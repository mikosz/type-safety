#pragma once

#include <array>

namespace type_safety {

class Vec3 {
public:

	Vec3() = default;

	Vec3(float x, float y, float z) :
		elements_{x, y, z}
	{
	}

	float get(size_t idx) const {
		return elements_[idx];
	}

	float& get(size_t idx) {
		return elements_[idx];
	}

private:

	std::array<float, 3> elements_;

};

class Vec4 {
public:

	Vec4() = default;

	Vec4(float x, float y, float z, float w) :
		elements_{x, y, z, w}
	{
	}

	Vec4(const Vec3& v, float w) :
		elements_{v.get(0), v.get(1), v.get(2), w}
	{
	}

	float get(size_t idx) const {
		return elements_[idx];
	}

	float& get(size_t idx) {
		return elements_[idx];
	}

private:

	std::array<float, 4> elements_;

};

} // namespace type_safety
