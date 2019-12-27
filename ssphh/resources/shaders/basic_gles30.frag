#version 300 es
#ifdef GL_ES
precision highp float;
#endif

// Uniforms

uniform vec3 Kd;
uniform sampler2D MapKd;

// Inputs

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vCameraPosition;
in vec3 vColor;

// Outputs

out vec4 oColor;

void main() {
	if (length(vColor) > 0.0) {
		oColor = vec4(vColor, 1.0);
		return;
	}
	vec3 kd = 0.5 * (texture(MapKd, vTexCoord).rgb + Kd);
	oColor = vec4(0.5 * vNormal + 0.5 + 0.5 * kd, 1.0);
}