#pragma once
#include <vector>
#include "Strawberry/Core/Math/Vector.hpp"


namespace Strawberry::UI
{
	class Glyph
	{
		friend class FontFace;

	public:
		using Index = unsigned int;


		struct Point
		{
			Core::Math::Vec2 position;
		};


		using Contour = std::vector<Point>;

		using ContourList = std::vector<Contour>;


		auto&& operator[](this auto&& self, size_t index) { return std::forward<decltype(self)>(self).mContours[index]; }


	private:
		explicit Glyph(Index index, ContourList points);


		Index mIndex;
		ContourList mContours;
	};
}
