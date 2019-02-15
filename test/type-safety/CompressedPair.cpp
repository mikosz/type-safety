#include <gtest/gtest.h>

#include "type-safety/CompressedPair.hpp"

namespace /* anonymous */ {

using namespace type_safety;

struct Empty {
	constexpr Empty() = default;
};

template <class Parent>
struct Child : Parent {
	bool b;
};

TEST(CompressedPairTest, BothEmptyIsEmpty) {
	using ChildType = Child<CompressedPair<Empty, Empty>>;
	static_assert(sizeof(ChildType) == sizeof(bool));
}

TEST(CompressedPairTest, NeitherEmptyHasSizeOfSum) {
	using ChildType = Child<CompressedPair<bool, bool>>;
	static_assert(sizeof(ChildType) == sizeof(bool) * 3);
}

TEST(CompressedPairTest, OneEmptyIsHasSizeOfSum) {
	using FirstEmptyChildType = Child<CompressedPair<bool, Empty>>;
	static_assert(std::is_same_v<CompressedPair<bool, Empty>::Stored, bool>);
	static_assert(sizeof(FirstEmptyChildType) == sizeof(bool) * 2);

	using SecondEmptyChildType = Child<CompressedPair<Empty, bool>>;
	static_assert(std::is_same_v<CompressedPair<Empty, bool>::Stored, bool>);
	static_assert(sizeof(SecondEmptyChildType) == sizeof(bool) * 2);
}

TEST(CompressedPairTest, BothEmptyIsConstexprDefaultConstructible) {
	[[maybe_unused]] constexpr auto p = CompressedPair<Empty, Empty>{};
}

TEST(CompressedPairTest, NeitherEmptyIsDefaultConstructible) {
	CompressedPair<int, int>{};
}

TEST(CompressedPairTest, OneEmptyDefaultConstructible) {
	CompressedPair<bool, Empty>{};
	CompressedPair<Empty, bool>{};
}

TEST(CompressedPairTest, NeitherEmptyIsConstructibleWithParameters) {
	const auto p = CompressedPair<int, int>{1, 2};
	EXPECT_EQ(p.first(), 1);
	EXPECT_EQ(p.second(), 2);
}

TEST(CompressedPairTest, OneEmptyIsConstructibleWithNonEmptyParameter) {
	const auto firstNonEmpty = CompressedPair<int, Empty>{42};
	static_assert(std::is_same_v<decltype(firstNonEmpty.first()), const int&>);
	static_assert(std::is_same_v<decltype(firstNonEmpty.second()), Empty>);
	EXPECT_EQ(firstNonEmpty.first(), 42);

	const auto secondNonEmpty = CompressedPair<Empty, int>{42};
	static_assert(std::is_same_v<decltype(secondNonEmpty.first()), Empty>);
	static_assert(std::is_same_v<decltype(secondNonEmpty.second()), const int&>);
	EXPECT_EQ(secondNonEmpty.second(), 42);
}

} // anonymous namespace
