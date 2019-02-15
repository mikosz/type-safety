#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <type_traits>

#include "type-safety/Xform.hpp"

using namespace type_safety;

namespace /* anonymous */ {

struct IntSpace {
	int marker;

	explicit IntSpace(int marker) :
		marker(marker)
	{
	}
};

bool spacesMatch(IntSpace lhs, IntSpace rhs) {
	return lhs.marker == rhs.marker;
}

struct WildcardSpace {
};

template <class SpaceT>
constexpr bool spaceTypesMatch(WildcardSpace, SpaceT) {
	return true;
}

 TEST(XformTest, XformEmptySpaceAccess) {
	auto xform = Xform<space::World, space::Player>{};
	auto from = xform.fromSpace();
	auto to = xform.toSpace();

	static_assert(std::is_same_v<decltype(from), space::World>);
	static_assert(std::is_same_v<decltype(to), space::Player>);

	static_assert(sizeof(xform) == sizeof(Matrix));
}

TEST(XformTest, XformFromIntToEmptySpaceAccess) {
	auto xform = Xform<IntSpace, space::Player>{IntSpace{42}};
	auto from = xform.fromSpace();
	auto to = xform.toSpace();

	static_assert(std::is_same_v<decltype(from), IntSpace>);
	static_assert(std::is_same_v<decltype(to), space::Player>);

	EXPECT_EQ(from.marker, 42);

	static_assert(sizeof(xform) == sizeof(Matrix) + sizeof(int));
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
	auto atp = Xform<WildcardSpace, space::Player>{};

	wtc.then(atp);
	atp.then(wtc);
}

TEST(XformTest, CanOverrideRuntimeSpaceMatching) {
	auto wti3 = Xform<space::World, IntSpace>{3};
	auto i3tp = Xform<IntSpace, space::Player>{3};
	auto i2tp = Xform<IntSpace, space::Player>{2};

	wti3.then(i3tp);
	wti3.then(i2tp);
}

} // anonymous namespace
