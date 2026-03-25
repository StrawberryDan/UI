#version 460


#include "TextNodeRenderer.glsl"


layout (location = 0) in vec2  position;
layout (location = 1) in vec2  size;
layout (location = 2) in uint  glyphAddressPageIndex;
layout (location = 3) in uvec2 glyphAddressCoordinate;
layout (location = 4) in uvec2 glyphAddressExtent;


layout (location=0) out flat vec2 out_glyphSampleCoordinates;
layout (location=1) out flat vec2 out_glyphExtent;
layout (location=2) out uint out_glyphPageIndex;
layout (location=3) out vec2 glyphCoordinates;

void main()
{
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    gl_Position.xy = position;

    out_glyphPageIndex = glyphAddressPageIndex;
    out_glyphSampleCoordinates = glyphAddressCoordinate;
    out_glyphExtent = glyphAddressExtent;

    switch (gl_VertexIndex)
    {
    case 0:
        glyphCoordinates = vec2(0, 0);
        break;
    case 1:
        gl_Position.x += size.x;
        glyphCoordinates = vec2(1, 0);
        break;
    case 2:
        gl_Position.xy += size;
        glyphCoordinates = vec2(1, 1);
        break;
    case 3:
        glyphCoordinates = vec2(0, 0);
        break;
    case 4:
        gl_Position.xy += size;
        glyphCoordinates = vec2(1, 1);
        break;
    case 5:
        gl_Position.y += size.y;
        glyphCoordinates = vec2(0, 1);
        break;
    };

    gl_Position = PROJECTION_MATRIX * gl_Position;
}