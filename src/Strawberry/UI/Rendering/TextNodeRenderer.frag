#version 460
#include "TextNodeRenderer.glsl"


layout (location=0) in flat vec2 glyphSampleCoordinates;
layout (location=1) in flat vec2 glyphExtent;
layout (location=2) in flat uint glyphPageIndex;
layout (location=3) in vec2 glyphCoordinates;


layout (location=0) out vec4 fragColor;


void main()
{
    vec2 textureCoordinate = glyphSampleCoordinates + glyphCoordinates * glyphExtent;

    textureCoordinate = textureCoordinate / PAGE_SIZE;

    vec4 fontSample = texture(
        usampler2DArray(FONT_ATLAS, FONT_ATLAS_SAMPLER),
        vec3(textureCoordinate, glyphPageIndex));

    fragColor = vec4(fontSample.r);
}