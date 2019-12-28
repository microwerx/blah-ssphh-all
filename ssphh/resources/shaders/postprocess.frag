#version 100
#ifdef GL_ES
precision highp float;
#endif

varying vec3 vTexCoord;

uniform float ToneMapExposure;
uniform float ToneMapGamma;
uniform float FilmicHighlights;
uniform float FilmicShadows;
uniform sampler2D MapDepthBuffer;
uniform sampler2D MapColorBuffer;
uniform sampler2D MapKd;
uniform sampler2D MapCorona;

vec3 srcColor1;
vec3 srcColor2;
float colorDifference;
int calcDifference = 1;

float relativeLuminance(vec3 c) {
	return dot(vec3(0.2126, 0.7152, 0.0722), c);
}

vec3 filmicCompress(vec3 linearColor) {
	vec3 x = max(vec3(0.0), linearColor - vec3(0.004));
	return (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06);
}

vec3 compareCorona() {
	srcColor1 = texture2D(MapColorBuffer, vTexCoord.st).rgb;
	srcColor2 = texture2D(MapCorona, vTexCoord.st).rgb;
	colorDifference = relativeLuminance(srcColor2) - relativeLuminance(srcColor1);
	if (colorDifference < 0.0) return vec3(0.0, min(1.0, -colorDifference), 0.0);
	return vec3(min(1.0, colorDifference), 0.0, 0.0);
}

void main() {
	vec3 color = vec3(0.1, 0.1, 0.0);
	
	color += srcColor1;
	//color += 0.3 * texture2D(MapKd, vTexCoord.st).rgb;
	
	float exposure = 2.5 * pow(2.0, ToneMapExposure);
	color *= exposure;

	if (calcDifference != 0) {
		vec3 compare = compareCorona();
		gl_FragColor = vec4(compare, 1.0);
	} else {
		if (ToneMapGamma > 0.0) {
			color = pow(color, vec3(1.0 / ToneMapGamma));	
		}

		if (FilmicHighlights > 0.0) {
			vec3 fcolor = filmicCompress(color);
			color = FilmicHighlights * fcolor + (1.0 - FilmicHighlights) * color;
		}

		gl_FragColor = vec4(color, 1.0);
	}
}