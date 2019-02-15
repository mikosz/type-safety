#pragma once

#include <type_traits>

#include "Matrix.hpp"
#include "CompressedPair.hpp"

namespace type_safety {

struct EmptySpace {
	struct ConstructorArgs {
	};
};

template <class FromSpaceT, class ToSpaceT>
constexpr bool spaceTypesMatch(FromSpaceT, ToSpaceT) {
	return std::is_same_v<FromSpaceT, ToSpaceT>;
}

template <class FromSpaceT, class ToSpaceT>
constexpr bool spacesMatch(FromSpaceT from_space, ToSpaceT to_space) {
	static_assert(
		spaceTypesMatch(from_space, to_space) || spaceTypesMatch(to_space, from_space),
		"Space types don't match"
	);
	return true;
}

template <class FromSpaceT, class ToSpaceT>
class Xform : CompressedPair<FromSpaceT, ToSpaceT> {
public:

	template <class... SpaceParams>
	Xform(SpaceParams&&... spaceParams) :
		CompressedPair<FromSpaceT, ToSpaceT>(std::forward<SpaceParams>(spaceParams)...)
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
			assert(false);
		}

		return true;
	}

private:

	Matrix matrix_;

};

namespace space {

struct World : EmptySpace {};
struct Camera : EmptySpace {};
struct Player : EmptySpace {};

} // namespace space

} // namespace type_safety
