	#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;

uniform mat4 transform;
uniform mat4 modelTransform;

const int MAX_BONES = 100;
uniform mat4 finalBonesMatrices[MAX_BONES];
 
out vec3 normal;
out vec2 texCoord;
out vec3 position;
 
void main() {
    mat4 skinMat =
          aWeights.x * finalBonesMatrices[aBoneIDs.x] +
          aWeights.y * finalBonesMatrices[aBoneIDs.y] +
          aWeights.z * finalBonesMatrices[aBoneIDs.z] +
          aWeights.w * finalBonesMatrices[aBoneIDs.w];

    vec4 skinnedPos    = skinMat * vec4(aPos, 1.0);
    vec3 skinnedNormal = mat3(skinMat) * aNormal;

    gl_Position = transform * skinnedPos;
    //gl_Position = transform * vec4(aPos, 1.0);
    //normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz;
    normal      = (transpose(inverse(modelTransform)) * vec4(skinnedNormal, 0.0)).xyz;
    texCoord = aTexCoord;
    //position = (modelTransform * vec4(aPos, 1.0)).xyz;
    position    = (modelTransform * skinnedPos).xyz;
}