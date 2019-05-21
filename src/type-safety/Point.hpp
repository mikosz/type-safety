#pragma once

#include <type_traits>

#include "VecN.hpp"
#include "math.hpp"

namespace type_safety {

template <class FromSpaceT, class ToSpaceT>
class Xform;

template <class SpaceT>
class Vector : SpaceT {
public:

	template <class... SpaceParams>
	Vector(SpaceParams&&... spaceParams) :
		SpaceT(std::forward<SpaceParams>(spaceParams)...)
	{
	}

	template <class... SpaceParams>
	Vector(Vec3 v, SpaceParams&&... spaceParams) :
		SpaceT(std::forward<SpaceParams>(spaceParams)...),
		vector_(std::move(v), 0.0f)
	{
	}

	template <class... SpaceParams>
	Vector(Vec4 v, SpaceParams&&... spaceParams) :
		SpaceT(std::forward<SpaceParams>(spaceParams)...),
		vector_(std::move(v))
	{
		assert(floatEq(v.get(3), 0.0f));
	}

	decltype(auto) space() const {
		if constexpr (std::is_empty_v<SpaceT>) {
			return SpaceT{};
		} else {
			return static_cast<const SpaceT&>(*this);
		}
	}

	Vec4& vector() {
		return vector_;
	}

	const Vec4& vector() const {
		return vector_;
	}

private:

	Vec4 vector_;

};

template <class SpaceT>
class Point : SpaceT {
public:

	template <class... SpaceParams>
	Point(SpaceParams&&... spaceParams) :
		SpaceT(std::forward<SpaceParams>(spaceParams)...)
	{
	}

	template <class... SpaceParams>
	Point(Vec3 v, SpaceParams&&... spaceParams) :
		SpaceT(std::forward<SpaceParams>(spaceParams)...),
		vector_(v, 1.0f)
	{
	}

	template <class... SpaceParams>
	Point(Vec4 v, SpaceParams&&... spaceParams) :
		SpaceT(std::forward<SpaceParams>(spaceParams)...),
		vector_(std::move(v))
	{
		assert(floatEq(v.get(3), 1.0f));
	}

	decltype(auto) space() const {
		if constexpr (std::is_empty_v<SpaceT>) {
			return SpaceT{};
		} else {
			return static_cast<const SpaceT&>(*this);
		}
	}

	Vec4& vector() {
		return vector_;
	}

	const Vec4& vector() const {
		return vector_;
	}

private:

	Vec4 vector_;

};

template <class SpaceT>
inline Vector<SpaceT> operator+(const Vector<SpaceT>& lhs, const Vector<SpaceT>& rhs) {
	checkSpacesMatch(lhs.space(), rhs.space());
	return Vector<SpaceT>{lhs.vector() + rhs.vector(), space()};
}

template <class SpaceT>
inline Vector<SpaceT> operator-(const Vector<SpaceT>& lhs, const Vector<SpaceT>& rhs) {
	checkSpacesMatch(lhs.space(), rhs.space());
	return Vector<SpaceT>{lhs.vector() - rhs.vector(), space()};
}

template <class SpaceT>
inline Vector<SpaceT> operator-(const Point<SpaceT>& lhs, const Point<SpaceT>& rhs) {
	checkSpacesMatch(lhs.space(), rhs.space());
	return Vector<SpaceT>{lhs.vector_ - rhs.vector_, space()};
}

template <class SpaceT>
inline Point<SpaceT> operator+(const Point<SpaceT>& lhs, const Vector<SpaceT>& rhs) {
	checkSpacesMatch(lhs.space(), rhs.space());
	return Point<SpaceT>{lhs.vector() + rhs.vector(), space()};
}

template <class SpaceT>
inline Point<SpaceT> operator+(const Vector<SpaceT>& lhs, const Point<SpaceT>& rhs) {
	checkSpacesMatch(lhs.space(), rhs.space());
	return Point<SpaceT>{lhs.vector() + rhs.vector(), space()};
}

} // namespace type_safety
