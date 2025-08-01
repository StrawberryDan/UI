#version 460


layout (location = 0) in flat uint OutDrawIndex;
layout (location = 1) in vec4 Color;

layout (location = 0) out vec4 FragColor;


layout (set=1,binding=0) uniform sampler2D mFontAtlas[256];


void main()
{
    FragColor = Color;
    gl_FragDepth = 1.0f - float(OutDrawIndex + 1.0f) / (1 << 16);
}