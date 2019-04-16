#pragma once

#include <array>

namespace type_safety {

class Vec4 {
public:

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
