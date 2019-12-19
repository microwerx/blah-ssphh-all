#version 300 es
#pragma optionNV(strict on)

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

precision highp float;
precision highp int; 

// Inputs from Vertex Shader ------------------------------------------

in vec4 vPosition;
in vec4 vCameraPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec4 vColor;


// Outputs from Fragment Shader ---------------------------------------

out vec4 oColor;

// Uniforms ----------------------------------------------------------

void main() {
	oColor = vec4(0.0, 1.0, 1.0, 1.0);
}