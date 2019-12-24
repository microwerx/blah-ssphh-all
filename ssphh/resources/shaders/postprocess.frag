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

vec3 filmicCompress(vec3 linearColor) {
	vec3 x = max(vec3(0.0), linearColor - vec3(0.004));
	return (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06);
}

void main() {
	vec3 color = vec3(0.1, 0.1, 0.0);

	color += texture2D(MapColorBuffer, vTexCoord.st).rgb;
	//color += 0.3 * texture2D(MapKd, vTexCoord.st).rgb;
	
	float exposure = 2.5 * pow(2.0, ToneMapExposure);
	color *= exposure;

	if (ToneMapGamma > 0.0) {
		color = pow(color, vec3(1.0 / ToneMapGamma));	
	}

	if (FilmicHighlights > 0.0) {
		vec3 fcolor = filmicCompress(color);
		color = FilmicHighlights * fcolor + (1.0 - FilmicHighlights) * color;
	}

	gl_FragColor = vec4(color, 1.0);
}