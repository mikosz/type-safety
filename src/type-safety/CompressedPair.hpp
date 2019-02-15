#pragma once

#include <type_traits>

namespace type_safety {

namespace detail {

template <class FirstT, class SecondT>
constexpr auto BOTH_EMPTY = std::is_empty_v<typename FirstT> && std::is_empty_v<typename SecondT>;

template <class FirstT, class SecondT>
constexpr auto NEITHER_EMPTY = !std::is_empty_v<typename FirstT> && !std::is_empty_v<typename SecondT>;

template <class FirstT, class SecondT>
constexpr auto ONE_EMPTY = !BOTH_EMPTY<FirstT, SecondT> && !NEITHER_EMPTY<FirstT, SecondT>;

template <class FirstT, class SecondT>
using NonEmptyT = std::conditional_t<!std::is_empty_v<typename FirstT>, FirstT, SecondT>;

} // namespace detail

template <class FirstT, class SecondT, class = void>
struct CompressedPair;

template <class FirstT, class SecondT>
struct CompressedPair<FirstT, SecondT, std::enable_if_t<detail::BOTH_EMPTY<FirstT, SecondT>>> {

	constexpr CompressedPair() = default;

	constexpr FirstT first() const {
		return FirstT{};
	}

	constexpr SecondT second() const {
		return SecondT{};
	}

};

template <class FirstT, class SecondT>
class CompressedPair<FirstT, SecondT, std::enable_if_t<detail::NEITHER_EMPTY<FirstT, SecondT>>> {
public:

	CompressedPair() = default;

	CompressedPair(FirstT first, SecondT second) :
		first_(std::move(first)),
		second_(std::move(second))
	{
	}

	const FirstT& first() const {
		return first_;
	}

	const SecondT& second() const {
		return second_;
	}

private:

	FirstT first_;

	SecondT second_;

};

template <class FirstT, class SecondT>
class CompressedPair<FirstT, SecondT, std::enable_if_t<detail::ONE_EMPTY<FirstT, SecondT>>> {
public:

	using Stored = detail::NonEmptyT<FirstT, SecondT>;

	CompressedPair() = default;

	CompressedPair(Stored stored) :
		stored_(std::move(stored))
	{
	}

	decltype(auto) first() const {
		if constexpr (!std::is_empty_v<FirstT>) {
			return static_cast<const FirstT&>(stored_);
		} else {
			return FirstT{};
		}
	}

	decltype(auto) second() const {
		if constexpr (!std::is_empty_v<SecondT>) {
			return static_cast<const SecondT&>(stored_);
		} else {
			return SecondT{};
		}
	}

private:

	Stored stored_;

};

} // namespace type_safety
