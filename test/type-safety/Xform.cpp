#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <type_traits>

#include "type-safety/Xform.hpp"

using namespace type_safety;

namespace /* anonymous */
{

 TEST(XformTest, XformEmptySpaceAccess) {
	auto xform = Xform<space::World, space::Player>{};
	auto from = xform.fromSpace();
	auto to = xform.toSpace();

	static_assert(std::is_same_v<decltype(from), space::World>);
	static_assert(std::is_same_v<decltype(to), space::Player>);

	static_assert(sizeof(xform) == sizeof(Matrix));
}

TEST(XformTest, XformFromIntToEmptySpaceAccess) {
	auto xform = makeXform(space::PlayerAtFrame{42}, space::Player{});
	auto from = xform.fromSpace();
	auto to = xform.toSpace();

	static_assert(std::is_same_v<decltype(from), space::PlayerAtFrame>);
	static_assert(std::is_same_v<decltype(to), space::Player>);

#ifdef DO_SPACE_RUNTIME_CHECKS
	EXPECT_EQ(from.frameId, 42);
	static_assert(sizeof(xform) == sizeof(Matrix) + sizeof(int));
#else
	static_assert(sizeof(xform) == sizeof(Matrix));
#endif /* DO_SPACE_RUNTIME_CHECKS */
}

TEST(XformTest, CanAppendXformsWithCompileTimeMatchingSpaces) {
	auto worldToCamera = Xform<space::World, space::Camera>{};
	auto cameraToPlayer = Xform<space::Camera, space::Player>{};

	inSequence(worldToCamera, cameraToPlayer);
	// Should not compile:
	// inSequence(cameraToPlayer, worldToCamera);
}

TEST(XformTest, CanOverrideCompileTimeSpaceMatching) {
	auto worldToCamera = Xform<space::World, space::Camera>{};
	auto AnyToPlayer = Xform<space::WildcardSpace, space::Player>{};
	auto playerToAny = Xform<space::Player, space::WildcardSpace>{};

	inSequence(worldToCamera, AnyToPlayer);
	inSequence(playerToAny, worldToCamera);
}

TEST(XformTest, CanOverrideRuntimeSpaceMatching) {
	auto worldToPlayerAt3 = makeXform(space::World{}, space::PlayerAtFrame{3});
	auto playerAt3ToPlayerAt2 = makeXform(space::PlayerAtFrame{3}, space::PlayerAtFrame{2});
	auto playerAt2ToPlayer = makeXform(space::PlayerAtFrame{2}, space::Player{});

	inSequence(inSequence(worldToPlayerAt3, playerAt3ToPlayerAt2), playerAt2ToPlayer);
}

#ifdef DO_SPACE_RUNTIME_CHECKS
TEST(XformTest, AssertsWhenRuntimeDataDoesntMatch) {
	auto worldToPlayerAt3 = makeXform(space::World{}, space::PlayerAtFrame{3});
	auto playerAt2ToPlayer = makeXform(space::PlayerAtFrame{2}, space::Player{});
	EXPECT_DEATH(inSequence(worldToPlayerAt3, playerAt2ToPlayer), "Run-time spaces don't match");
}
#endif /* DO_SPACE_RUNTIME_CHECKS */

TEST(XformTest, ImplicitlyConvertibleSpacesMatch) {
	auto worldToPlayer = makeXform(space::World{}, space::Player{});
	auto playerAt1ToPlayerAt2 = makeXform(space::PlayerAtFrame{1}, space::PlayerAtFrame{2});
	auto playerToCamera = Xform<space::Player, space::Camera>{};

	inSequence(inSequence(worldToPlayer, playerAt1ToPlayerAt2), playerToCamera);
}

} // anonymous namespace
