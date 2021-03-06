#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform GlobalUniforms {
    vec2 resolution;
} global;

layout(set = 2, binding = 0) uniform CameraObject {
    mat4 view;
    mat4 perspective;
    mat4 viewPerspective;
    mat4 orientation;
    vec4 position;
} camera;

layout(set = 6, binding = 0) uniform ObjectData {
    mat4 MVP;
} objectData;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = objectData.MVP * vec4(inPosition, 1.0);
    fragColor = inColor;
	fragTexCoord = inTexCoord;
}
