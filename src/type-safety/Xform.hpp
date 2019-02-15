#pragma once

#include <type_traits>

#include "Matrix.hpp"

namespace type_safety {

struct EmptySpace {
	struct ConstructorArgs {
	};
};

//template <class FromSpaceT, class ToSpaceT>
//class Xform : detail::FromToStorage<FromSpaceT, ToSpaceT> {
//public:
//
//	using detail::FromToStorage<FromSpaceT, ToSpaceT>::fromSpace;
//	using detail::FromToStorage<FromSpaceT, ToSpaceT>::toSpace;
//
//	Xform(
//		typename FromSpaceT::ConstructorArgs fromSpaceArgs,
//		typename ToSpaceT::ConstructorArgs toSpaceArgs
//	) :
//		detail::FromToStorage<FromSpaceT, ToSpaceT>(std::move(fromSpaceArgs), std::move(toSpaceArgs))
//	{
//	}
//
//	template <class OtherFromSpaceT, class OtherToSpaceT>
//	auto then(const Xform<OtherFromSpaceT, OtherToSpaceT>& other) const {
//		static_assert(compileTimeSpacesMatch<ToSpaceT, OtherFromSpaceT>());
//		if (!runtimeSpacesMatch(toSpace(), other.fromSpace())) {
//			assert(false);
//		}
//
//		return true;
//	}
//
//private:
//
//	Matrix matrix_;
//
//};

namespace space {

struct World : EmptySpace {};
struct Camera : EmptySpace {};
struct Player : EmptySpace {};

} // namespace space

} // namespace type_safety
