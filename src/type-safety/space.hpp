#pragma once

namespace type_safety {

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

namespace space {

struct World {};
struct Camera {};
struct Player {};

} // namespace space

} // namespace type_safety
