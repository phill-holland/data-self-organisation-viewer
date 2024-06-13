#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform camera 
{
    mat4 world;
    mat4 object;
} cameraData; 

layout(push_constant) uniform constants
{
    mat4 render_matrix;
} PushConstants;

struct ObjectData {
    mat4 position;
    mat4 matrix;
};

layout(std140, set = 1, binding = 0) readonly buffer ObjectBuffer {
    ObjectData objects[];
} objectBuffer;

void main() {
    gl_Position = PushConstants.render_matrix * (cameraData.world * (objectBuffer.objects[gl_InstanceIndex].position * cameraData.object * (objectBuffer.objects[gl_InstanceIndex].matrix * vec4(inPosition, 1.0))));
    fragColor = inColor;
}
