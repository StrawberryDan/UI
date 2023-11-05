#version 450

layout(push_constant) uniform PushConstants
{
    vec2 rectPos;
    vec2 rectSize;
    vec4 fillColor;
};

layout(location=0) out vec4 fragColor;

void main() {
    fragColor = fillColor;
}
