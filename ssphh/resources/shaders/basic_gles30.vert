#version 300 es

// Uniforms

layout(std140) uniform CameraBlock {
	mat4 ProjectionMatrix;
	mat4 CameraMatrix;
};

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

void main(void)
{
	vPosition = (WorldMatrix * vec4(aPosition, 1.0)).xyz;
	vCameraPosition = CameraMatrix[3].xyz;
	vTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
	vNormal = mat3(WorldMatrix) * aNormal;
	vColor = aColor;

	mat4 pcw = ProjectionMatrix * CameraMatrix * WorldMatrix;
	gl_Position = pcw * vec4(aPosition, 1.0);
}
