#version 300 es
precision highp float;

in vec3 vTexCoord;

// uniform float ToneMapExposure;
// uniform float ToneMapGamma;
// uniform float FilmicHighlights;
// uniform float FilmicShadows;

layout(std140) uniform EnvironmentBlock {
	vec4 ToneMap;		// {r} = exposure, {g} = gamma, {b} = filmic highlight, {a} = filmic shadows
	vec4 FogSH;			// {r} = L0, {g} = L1,M-1, {b} = L1,M0, {a} = L1,M1
	vec4 Ground;		// {rgb} = ground color, {a} = ground luminance
	vec4 Location;		// {r} = latitude, {g} = longitude, {b} = altitude, {a} = ground altitude
	vec4 Date;			// {r} = year, {g} = month, {b} = day, {a} = jday
	vec4 Time;			// {r} = hour, {g} = minutes, {b} = seconds, {a} day seconds
	vec4 Sun;			// {rgb} = disk radiance, {a} = solid angle
	vec4 Moon;			// {rgb} = disk radiance, {a} = solid angle
};

uniform sampler2D MapDepthBuffer;
uniform sampler2D MapColorBuffer;
uniform sampler2D MapKd;
uniform sampler2D MapCorona;

#define ToneMapExposure ToneMap.r
#define ToneMapGamma    ToneMap.g
#define ToneMapFilmicHighlight ToneMap.b
#define ToneMapFilmicShadows   ToneMap.a

vec3 srcColor1;
vec3 srcColor2;
float colorDifference;
int calcDifference = 0;

float relativeLuminance(vec3 c) {
	return dot(vec3(0.2126, 0.7152, 0.0722), c);
}

vec3 filmicCompress(vec3 linearColor) {
	vec3 x = max(vec3(0.0), linearColor - vec3(0.004));
	return (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06);
}

vec3 calcToneMap(vec3 color) {
	float exposure = 2.5 * pow(2.0, ToneMapExposure);
	color *= exposure;

	if (ToneMapGamma > 0.0) {
		color = pow(color, vec3(1.0 / ToneMapGamma));	
	}

	if (ToneMapFilmicHighlight > 0.0) {
		vec3 fcolor = filmicCompress(color);
		color = mix(color, fcolor, ToneMapFilmicHighlight);
	}
	return color;
}

vec3 compareCorona() {
	srcColor2 = texture(MapCorona, vTexCoord.st).rgb;
	colorDifference = relativeLuminance(srcColor2) - relativeLuminance(srcColor1);
	if (colorDifference < 0.0) return vec3(0.0, min(1.0, -colorDifference), 0.0);
	return vec3(min(1.0, colorDifference), 0.0, 0.0);
}

out vec4 oColor;

void main() {
	vec3 color = vec3(0.1, 0.1, 0.0);
	
	srcColor1 = texture(MapColorBuffer, vTexCoord.st).rgb;
	color += srcColor1;
	//color += 0.3 * texture2D(MapKd, vTexCoord.st).rgb;
	
	if (calcDifference != 0) {
		vec3 compare = compareCorona();
		oColor = vec4(compare, 1.0);
	} else {
		oColor = vec4(calcToneMap(color), 1.0);
	}
}