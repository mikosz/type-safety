#pragma once

#include <type_traits>

#include "CompressedPair.hpp"
#include "Matrix.hpp"
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

	decltype(auto) fromSpace() const {
		return CompressedPair<FromSpaceT, ToSpaceT>::first();
	}

	decltype(auto) toSpace() const {
		return CompressedPair<FromSpaceT, ToSpaceT>::second();
	}

	template <class OtherFromSpaceT, class OtherToSpaceT>
	auto then(const Xform<OtherFromSpaceT, OtherToSpaceT>& other) const {
		if (!spacesMatch(toSpace(), other.fromSpace())) {
			assert(!"Run-time spaces don't match");
		}

		return Xform<FromSpaceT, OtherToSpaceT>{matrix_ * other.matrix(), fromSpace(), other.toSpace()};
	}

	const Matrix& matrix() const {
		return matrix_;
	}

private:

	Matrix matrix_;

};

template <class FromSpace, class ToSpace>
auto makeXform(FromSpace fromSpace, ToSpace toSpace) {
	return Xform<FromSpace, ToSpace>{std::move(fromSpace), std::move(toSpace)};
}

template <class FromSpace, class ToSpace>
auto makeXform(Matrix matrix, FromSpace fromSpace, ToSpace toSpace) {
	return Xform<FromSpace, ToSpace>{std::move(matrix), std::move(fromSpace), std::move(toSpace)};
}

} // namespace type_safety
