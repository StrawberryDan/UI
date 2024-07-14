#include "Glyph.hpp"


namespace Strawberry::UI
{
	Glyph::Glyph(Contours points)
		: mContours(std::move(points)) {}
}
