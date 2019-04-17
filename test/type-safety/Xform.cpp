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
	auto xform = Xform<space::PlayerAtFrame, space::Player>{space::PlayerAtFrame{42}, space::Player{}};
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
	auto wtc = Xform<space::World, space::Camera>{};
	auto ctp = Xform<space::Camera, space::Player>{};

	wtc.then(ctp);
	// Should not compile:
	// ctp.then(wtc);
}

TEST(XformTest, CanOverrideCompileTimeSpaceMatching) {
	auto wtc = Xform<space::World, space::Camera>{};
	auto atp = Xform<space::WildcardSpace, space::Player>{};
	auto pta = Xform<space::Player, space::WildcardSpace>{};

	wtc.then(atp);
	pta.then(wtc);
}

TEST(XformTest, CanOverrideRuntimeSpaceMatching) {
	auto wtp3 = Xform<space::World, space::PlayerAtFrame>{space::World{}, space::PlayerAtFrame{3}};
	auto p3tp2 = Xform<space::PlayerAtFrame, space::PlayerAtFrame>{space::PlayerAtFrame{3}, space::PlayerAtFrame{2}};
	auto p2tp = Xform<space::PlayerAtFrame, space::Player>{space::PlayerAtFrame{2}, space::Player{}};

	wtp3.then(p3tp2).then(p2tp);
}

#ifdef DO_SPACE_RUNTIME_CHECKS
TEST(XformTest, AssertsWhenRuntimeDataDoesntMatch) {
	auto wtp3 = Xform<space::World, space::PlayerAtFrame>{space::World{}, space::PlayerAtFrame{3}};
	auto p2tp = Xform<space::PlayerAtFrame, space::Player>{space::PlayerAtFrame{2}, space::Player{}};
	EXPECT_DEATH(wtp3.then(p2tp), "Run-time spaces don't match");
}
#endif /* DO_SPACE_RUNTIME_CHECKS */

TEST(XformTest, ImplicitlyConvertibleSpacesMatch) {
	auto wtp = Xform<space::World, space::Player>{space::World{}, space::Player{}};
	auto p1tp2 = Xform<space::PlayerAtFrame, space::PlayerAtFrame>{space::PlayerAtFrame{1}, space::PlayerAtFrame{2}};
	auto ptc = Xform<space::Player, space::Camera>{};

	wtp.then(p1tp2).then(ptc);
}

} // anonymous namespace
