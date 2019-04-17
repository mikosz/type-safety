#pragma once

// Comment me out to disable space runtime checks
#define DO_SPACE_RUNTIME_CHECKS

namespace type_safety {

template <class FromSpaceT, class ToSpaceT>
constexpr bool SpaceTypesMatch_v =
	std::is_same_v<FromSpaceT, ToSpaceT> ||
	std::is_convertible_v<FromSpaceT, ToSpaceT>;

template <class FromSpaceT, class ToSpaceT>
constexpr bool spacesMatch(FromSpaceT, ToSpaceT) {
	static_assert(
		SpaceTypesMatch_v<FromSpaceT, ToSpaceT> || SpaceTypesMatch_v<ToSpaceT, FromSpaceT>,
		"Space types don't match"
		);
	return true;
}

namespace space {

struct World {};
struct Camera {};
struct Player {};

struct PlayerAtFrame : space::Player {
#ifdef DO_SPACE_RUNTIME_CHECKS
	int frameId;

	explicit PlayerAtFrame(int frameId) :
		frameId(frameId)
	{
	}
#else
	PlayerAtFrame() = default;

	PlayerAtFrame([[maybe_unused]] int frameId) {
	}
#endif /* DO_SPACE_RUNTIME_CHECKS */
};

#ifdef DO_SPACE_RUNTIME_CHECKS
bool spacesMatch(PlayerAtFrame lhs, PlayerAtFrame rhs) {
	return lhs.frameId == rhs.frameId;
}
#endif /* DO_SPACE_RUNTIME_CHECKS */

struct WildcardSpace {
};

} // namespace space

template <class SpaceT>
constexpr bool SpaceTypesMatch_v<space::WildcardSpace, SpaceT> = true;

} // namespace type_safety
