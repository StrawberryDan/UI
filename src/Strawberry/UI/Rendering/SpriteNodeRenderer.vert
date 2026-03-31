#version 460

#include "SpriteNodeRenderer.glsl"


layout (location=0) in vec2 inPosition;
layout (location=1) in vec2 inExtent;
layout (location=2) in vec2 inTextureMin;
layout (location=3) in vec2 inTextureMax;
layout (location=4) in uint inTexturePageIndex;

layout (location=0) out vec2 outTextureCoord;
layout (location=1) out flat uint outTexturePageIndex;

const vec2 GEOMETRY[] =
{
    vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1, 0),
    vec2(1, 1),
    vec2(0, 1)
};

vec2 GetTextureCoordinate(vec2 value)
{
    switch (gl_VertexIndex)
    {
        case 0:
            return inTextureMin;
        case 1:
        case 3:
        return vec2(inTextureMax.x, inTextureMin.y);
        case 4:
            return inTextureMax;
        case 2:
        case 5:
        return vec2(inTextureMin.x, inTextureMax.y);
    }
}

void main()
{
    gl_Position = ProjectionMatrix * vec4(inPosition + inExtent * GEOMETRY[gl_VertexIndex], 0.0, 1.0);
    outTextureCoord = GetTextureCoordinate(gl_Position.xy);
    outTexturePageIndex = inTexturePageIndex;
}