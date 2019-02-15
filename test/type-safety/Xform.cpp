#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <type_traits>

#include "type-safety/Xform.hpp"

using namespace type_safety;

namespace /* anonymous */ {

struct IntSpace {
	int marker;
};

//TEST(XformTest, XformEmptySpaceAccess) {
//	auto xform = Xform<space::World, space::Player>{};
//	auto from = xform.fromSpace();
//	auto to = xform.toSpace();
//
//	static_assert(std::is_same_v<decltype(from), space::World>);
//	static_assert(std::is_same_v<decltype(to), space::Player>);
//
//	static_assert(sizeof(xform) == sizeof(Matrix));
//}
//
//TEST(XformTest, XformFromIntToEmptySpaceAccess) {
//	auto xform = Xform<IntSpace, space::Player>{};
//	auto from = xform.fromSpace();
//	auto to = xform.toSpace();
//
//	static_assert(std::is_same_v<decltype(from), IntSpace>);
//	static_assert(std::is_same_v<decltype(to), space::Player>);
//
//	static_assert(sizeof(xform) == sizeof(Matrix));
//}

} // anonymous namespace
