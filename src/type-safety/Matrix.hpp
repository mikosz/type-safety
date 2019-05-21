#pragma once

#include <array>

#include "VecN.hpp"

namespace type_safety {

class Matrix {
public:

	friend void multiplyAndSet(Matrix& result, const Matrix& lhs, const Matrix& rhs) {
		for (auto row = 0u; row < 4u; ++row) {
			for (auto col = 0u; col < 4u; ++col) {
				auto value = 0.0f;
				for (auto dot = 0u; dot < 4u; ++dot) {
					value += lhs.elements_[index(row, dot)] * rhs.elements_[index(dot, col)];
				}
				result.elements_[index(row, col)] = value;
			}
		}
	}

	friend void multiplyAndSet(Vec4& result, const Matrix& lhs, const Vec4& rhs) {
		for (auto row = 0u; row < 4u; ++row) {
			auto value = 0.0f;
			for (auto col = 0u; col < 4u; ++col) {
				value += lhs.get(row, col) * rhs.get(col);
			}
			result.get(row) = value;
		}
	}

	friend Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
		auto result = Matrix{};
		multiplyAndSet(result, lhs, rhs);
		return result;
	}

	friend Vec4 operator*(const Matrix& lhs, const Vec4& rhs) {
		auto result = Vec4{};
		multiplyAndSet(result, lhs, rhs);
		return result;
	}

	Matrix& operator*=(const Matrix& other) {
		auto result = *this * other;
		*this = result;
		return *this;
	}

	float& get(size_t row, size_t column) {
		return elements_[index(row, column)];
	}

	const float get(size_t row, size_t column) const {
		return elements_[index(row, column)];
	}

private:

	std::array<float, 16> elements_;

	static size_t index(size_t row, size_t column) {
		return row * 4u + column;
	}

};

} // namespace type_safety
