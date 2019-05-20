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
		// TODO: ignoring space order here makes it impossible for us to crate spaces where it would be
		// possible to implicitly convert them one way but not another. E.g. PlayerAtFrame should degenerate
		// to Player, but not the other way round. FIX IT!
		SpaceTypesMatch_v<FromSpaceT, ToSpaceT> || SpaceTypesMatch_v<ToSpaceT, FromSpaceT>,
		"Space types don't match"
		);
	return true;
}

namespace space {

struct World {};
struct Camera {};
struct Player {};

struct PlayerAtFrame : Player {
#ifdef DO_SPACE_RUNTIME_CHECKS
	int frameId;

	explicit PlayerAtFrame(int frameId) :
		frameId(frameId)
	{
	}
#else
	constexpr PlayerAtFrame() = default;

	constexpr explicit PlayerAtFrame([[maybe_unused]] int frameId) {
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

template <class LhsSpaceT, class RhsSpaceT>
inline void checkSpacesMatch(
	[[maybe_unused]] const LhsSpaceT& lhs,
	[[maybe_unused]] const RhsSpaceT& rhs
) {
#ifdef DO_SPACE_RUNTIME_CHECKS
	if (!spacesMatch(lhs, rhs)) {
		throw std::runtime_error("Run-time spaces don't match");
	}
#else
	static_assert(spacesMatch(LhsSpaceT{}, RhsSpaceT{}), "Space types don't match");
#endif /* DO_SPACE_RUNTIME_CHECKS */
}

} // namespace type_safety
