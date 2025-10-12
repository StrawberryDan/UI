#version 460
#include "TextNodeRenderer.glsl"


layout (location=0) in flat Glyph glyph;
layout (location=4) in vec2 glyphCoordinates;


layout (location=0) out vec4 fragColor;


void main()
{
    vec2 textureCoordinate = glyph.glyphAddressCoordinate + glyphCoordinates * glyph.size;

    textureCoordinate.x = textureCoordinate.x / PAGE_SIZE.x;
    textureCoordinate.y = textureCoordinate.y / PAGE_SIZE.y;

    vec4 fontSample = texture(
        usampler2DArray(FONT_ATLAS, _sampler),
        vec3(textureCoordinate, glyph.glyphAddressPageIndex));

    fragColor.r = fontSample.r;
    fragColor.g = fontSample.r;
    fragColor.b = fontSample.r;
    fragColor.a = fontSample.r;
}