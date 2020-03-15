#version 300 es

#define MAX_DIRTOLIGHT_COUNT	8

// Uniforms

layout(std140) uniform CameraBlock {
	mat4 ProjectionMatrix;
	mat4 CameraMatrix;
};

uniform mat4 DirToShadowMatrix[MAX_DIRTOLIGHT_COUNT];

uniform mat4 WorldMatrix;

// Inputs

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aColor;

// Outputs

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vCameraPosition;
out vec3 vColor;
out vec4 vShadowCoords[MAX_DIRTOLIGHT_COUNT];

void main(void)
{
	vPosition = (WorldMatrix * vec4(aPosition, 1.0)).xyz;
	vCameraPosition = CameraMatrix[3].xyz;
	vTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
	vNormal = mat3(WorldMatrix) * aNormal;
	vColor = aColor;

	vShadowCoords[0] = DirToShadowMatrix[0] * vec4(vPosition, 1.0);
	vShadowCoords[1] = DirToShadowMatrix[1] * vec4(vPosition, 1.0);
	vShadowCoords[2] = DirToShadowMatrix[2] * vec4(vPosition, 1.0);
	vShadowCoords[3] = DirToShadowMatrix[3] * vec4(vPosition, 1.0);

	mat4 pcw = ProjectionMatrix * CameraMatrix * WorldMatrix;
	gl_Position = pcw * vec4(aPosition, 1.0);
}
