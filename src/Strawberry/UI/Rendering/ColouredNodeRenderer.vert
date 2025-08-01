#version 460


const vec2 Positions[6] =
{
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(1.0, 1.0),
    vec2(0.0, 0.0),
    vec2(1.0, 1.0),
    vec2(0.0, 1.0),
};


layout (location=0) in uint DrawIndex;
layout (location=1) in vec2 Position;
layout (location=2) in vec2 Extent;
layout (location=3) in vec4 Color;


layout (location=0) out uint OutDrawIndex;
layout (location=1) out vec4 OutColor;


layout (set=0,binding=0) uniform RenderConstants
{
    mat4 Projection;
};


void main()
{
    OutColor = Color;
    OutDrawIndex = DrawIndex;

    gl_Position = vec4(Position, 0.0, 1.0);

    gl_Position = gl_Position + vec4(Positions[gl_VertexIndex] * Extent, 0.0f, 0.0f);

    gl_Position = Projection * gl_Position;
}