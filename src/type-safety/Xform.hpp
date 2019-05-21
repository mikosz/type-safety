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
		checkSpacesMatch(fromSpace(), p.space());
		auto result = Point<ToSpaceT>{toSpace()};
		multiplyAndSet(result.vector(), matrix_, p.vector());
		return result;
	}

	Vector<ToSpaceT> apply(const Vector<FromSpaceT>& v) const {
		checkSpacesMatch(fromSpace(), v.space());
		auto result = Vector<ToSpaceT>{toSpace()};
		multiplyAndSet(result.vector(), matrix_, v.vector());
		return result;
	}

	decltype(auto) fromSpace() const {
		return CompressedPair<FromSpaceT, ToSpaceT>::first();
	}

	decltype(auto) toSpace() const {
		return CompressedPair<FromSpaceT, ToSpaceT>::second();
	}

	Matrix& matrix() {
		return matrix_;
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
	auto result = Xform<LhsFromSpaceT, RhsToSpaceT>{};
	multiplyAndSet(result.matrix(), rhs.matrix(), lhs.matrix());
	return result;
}

} // namespace type_safety
