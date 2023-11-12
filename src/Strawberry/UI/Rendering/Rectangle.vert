#version 450

vec2 vertices[4] = {
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 1.0)
};

layout(set=0,binding=0) uniform Constants
{
    mat4 viewMatrix;
};

layout(push_constant) uniform PushConstants
{
    vec2 rectPos;
    vec2 rectSize;
    vec4 fillColor;
};

void main() {
    vec2 position = vertices[gl_VertexIndex];
    position = rectSize * position;
    position = position + rectPos;
    gl_Position = viewMatrix * vec4(position, 0.0, 1.0);
}
