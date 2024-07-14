#include "Glyph.hpp"


namespace Strawberry::UI
{
	Glyph::Glyph(ContourList points)
		: mContours(std::move(points)) {}
}
