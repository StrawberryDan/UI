#version 460

#include "SpriteNodeRenderer.glsl"

layout (location=0) in vec2 inTextureCoord;
layout (location=1) in flat uint inTexturePageIndex;

layout (location = 0) out vec4 outFragColor;

void main()
{
    outFragColor = texture(DiffuseTexture, vec3(inTextureCoord, inTexturePageIndex));
}