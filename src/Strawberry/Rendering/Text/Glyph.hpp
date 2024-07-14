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


		using PointList = std::vector<Point>;

		using Contours = std::vector<PointList>;

	private:
		explicit Glyph(Contours points);


		Contours mContours;
	};
}
