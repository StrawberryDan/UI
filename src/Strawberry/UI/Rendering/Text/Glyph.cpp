#include "Glyph.hpp"


namespace Strawberry::UI
{
	Glyph::Glyph(Index index, ContourList points)
		: mIndex(index)
		, mContours(std::move(points)) {}
}
