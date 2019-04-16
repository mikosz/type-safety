#pragma once

namespace type_safety {

template <class SpaceT>
class Vector : SpaceT {
public:

	template <class... SpaceParams>
	Vector(SpaceParams&&... spaceParams) :
		SpaceT(std::forward<SpaceParams>(spaceParams)...)
	{
	}

};

class Point {
public:
};

} // namespace type_safety
