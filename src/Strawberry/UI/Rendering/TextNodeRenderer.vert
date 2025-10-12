#version 460


#include "TextNodeRenderer.glsl"




layout (location=0) out flat Glyph glyph;
layout (location=4) out vec2 glyphCoordinates;


void main()
{
    glyph = GLYPHS[gl_InstanceIndex];
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    gl_Position.xy = glyph.position;
    switch (gl_VertexIndex)
    {
    case 0:
        glyphCoordinates = vec2(0, 0);
        break;
    case 1:
        gl_Position.x += glyph.size.x;
        glyphCoordinates = vec2(1, 0);
        break;
    case 2:
        gl_Position.xy += glyph.size;
        glyphCoordinates = vec2(1, 1);
        break;
    case 3:
        glyphCoordinates = vec2(0, 0);
        break;
    case 4:
        gl_Position.xy += glyph.size;
        glyphCoordinates = vec2(1, 1);
        break;
    case 5:
        gl_Position.y += glyph.size.y;
        glyphCoordinates = vec2(0, 1);
        break;
    };

    gl_Position = PROJECTION_MATRIX * gl_Position;
}