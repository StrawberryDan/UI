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


		/// Struct representing a point in a glyph's contour.
		struct Point
		{
			Core::Math::Vec2 position;
		};


		/// The contour of the glyph.
		using Contour = std::vector<Point>;

		/// The list of contours in this glyph
		using ContourList = std::vector<Contour>;


		/// Index access into the contour list.
		auto operator[](this auto self, size_t index) { return self.mContours[index]; }


	private:
		explicit Glyph(Index index, ContourList points);


		/// The index of this glyph in its font face.
		Index mIndex;
		/// This glyph's contours.
		ContourList mContours;
	};
}
