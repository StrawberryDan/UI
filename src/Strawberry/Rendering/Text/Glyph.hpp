#pragma once
#include <vector>
#include "Strawberry/Core/Math/Vector.hpp"


namespace Strawberry::UI
{
	class Glyph
	{
		friend class FontFace;

	public:
		struct Point
		{
			Core::Math::Vec2 position;
		};


		using Contour = std::vector<Point>;

		using ContourList = std::vector<Contour>;


		auto&& operator[](this auto&& self, size_t index) { return std::forward<decltype(self)>(self).mContours[index]; }


	private:
		explicit Glyph(ContourList points);


		ContourList mContours;
	};
}
