#version 300 es

// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com

// Uniforms

uniform mat4 ProjectionMatrix;
uniform mat4 InverseCameraMatrix;
uniform mat4 CameraMatrix;
uniform mat4 ModelViewMatrix;
uniform vec4 CameraPosition;

// Inputs

in vec4 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;
in vec4 aColor;
in vec4 aAttrib;

// Outputs

out vec4 vPosition;
out vec4 vCameraPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec4 vColor;
out vec3 vAttrib;
out vec3 vTangent;
out vec3 vBinormal;
out vec4 vSunShadowCoord;
out vec3 vAmbient;

void main(void)
{
	mat3 NormalMatrixWorldSpace = mat3(ModelViewMatrix);
	mat3 NormalMatrixCamSpace = mat3(CameraMatrix);
	mat3 NormalMatrix = NormalMatrixWorldSpace;
	mat3 NormalCameraMatrix = NormalMatrixCamSpace * NormalMatrixWorldSpace;

	mat4 ProjectionModelViewMatrix = ProjectionMatrix * CameraMatrix * ModelViewMatrix;

	vPosition = ModelViewMatrix * aPosition;
	vCameraPosition = vec4(CameraMatrix[3].xyz, 1.0);;
	vTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
	vNormal = NormalMatrix * aNormal;
	vColor = aColor;

	gl_Position = ProjectionModelViewMatrix * aPosition;
}
