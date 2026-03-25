layout (set = 0, binding = 0) uniform RenderConstants
{
    mat4  PROJECTION_MATRIX;
    uvec2 PAGE_SIZE;
};

layout (set = 0, binding = 1) uniform utexture2DArray FONT_ATLAS;
layout (set = 0, binding = 2) uniform sampler FONT_ATLAS_SAMPLER;