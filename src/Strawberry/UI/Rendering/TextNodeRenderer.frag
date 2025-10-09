#version 460
#include "TextNodeRenderer.glsl"


layout (location=0) in flat Glyph glyph;
layout (location=4) in vec2 glyphCoordinates;


layout (location=0) out vec4 fragColor;


void main()
{
    fragColor = textureLod(
        usampler2DArray(FONT_ATLAS, _sampler),
        uvec3(glyph.glyphAddressCoordinate + glyphCoordinates * glyph.size, glyph.glyphAddressPageIndex),
        0);
}