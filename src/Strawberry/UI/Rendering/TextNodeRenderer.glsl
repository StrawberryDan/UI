const uint ATLAS_SIZE = 8;

struct Glyph
{
    uvec2 position;
    uvec2 size;
    uint glyphAddressPageIndex;
    uvec2 glyphAddressCoordinate;
};


layout (set = 0, binding = 0) readonly buffer GlyphBuffer
{
    Glyph GLYPHS[];
};


layout (set = 1, binding = 0) uniform RenderConstants
{
    mat4  PROJECTION_MATRIX;
};

layout (set = 1, binding = 1) uniform utexture2DArray FONT_ATLAS;
layout (set = 1, binding = 2) uniform sampler _sampler;