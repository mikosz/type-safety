#pragma once

#include <type_traits>
#include <stdexcept>

#include "CompressedPair.hpp"
#include "Matrix.hpp"
#include "Point.hpp"
#include "space.hpp"

namespace type_safety {

template <class FromSpaceT, class ToSpaceT>
class Xform : CompressedPair<FromSpaceT, ToSpaceT> {
public:

	template <class... SpaceParams>
	Xform(SpaceParams&&... spaceParams) :
		CompressedPair<FromSpaceT, ToSpaceT>(std::forward<SpaceParams>(spaceParams)...)
	{
	}

	template <class... SpaceParams>
	Xform(Matrix matrix, SpaceParams&&... spaceParams) :
		CompressedPair<FromSpaceT, ToSpaceT>(std::forward<SpaceParams>(spaceParams)...),
		matrix_(std::move(matrix))
	{
	}

	Point<ToSpaceT> apply(const Point<FromSpaceT>& p) const {
		checkSpacesMatch(fromSpace(), p.space())
		return Point<ToSpaceT>{matrix_ * p.vector(), toSpace()};
	}

	Vector<ToSpaceT> apply(const Vector<FromSpaceT>& v) const {
		checkSpacesMatch(fromSpace(), v.space());
		return Vector<ToSpaceT>{matrix_ * v.vector(), toSpace()};
	}

	decltype(auto) fromSpace() const {
		return CompressedPair<FromSpaceT, ToSpaceT>::first();
	}

	decltype(auto) toSpace() const {
		return CompressedPair<FromSpaceT, ToSpaceT>::second();
	}

	const Matrix& matrix() const {
		return matrix_;
	}

private:

	Matrix matrix_;

};

template <class FromSpace, class ToSpace>
inline auto makeXform(FromSpace fromSpace, ToSpace toSpace) {
	return Xform<FromSpace, ToSpace>{std::move(fromSpace), std::move(toSpace)};
}

template <class FromSpace, class ToSpace>
inline auto makeXform(Matrix matrix, FromSpace fromSpace, ToSpace toSpace) {
	return Xform<FromSpace, ToSpace>{std::move(matrix), std::move(fromSpace), std::move(toSpace)};
}

template <class LhsFromSpaceT, class LhsToSpaceT, class RhsFromSpaceT, class RhsToSpaceT>
inline auto inSequence(
	const Xform<LhsFromSpaceT, LhsToSpaceT>& lhs,
	const Xform<RhsFromSpaceT, RhsToSpaceT>& rhs
) {
	checkSpacesMatch(lhs.toSpace(), rhs.fromSpace());
	return Xform<LhsFromSpaceT, RhsToSpaceT>{rhs.matrix() * lhs.matrix(), lhs.fromSpace(), rhs.toSpace()};
}

} // namespace type_safety
