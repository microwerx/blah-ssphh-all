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
	vec4 CompareBlend;	// {r} = fade image, {g} = fade color, {b} = fade reference, {a} = image difference
	vec4 FadeColor;	    // {rgb} = color, {a} = fade dissolve
	vec4 ReferenceOps;	// {r} = exposure, {g} = gamma, {b} = filmic highlight, {a} = filmic shadows
	vec4 Flips;			// {r} = flip X, {g} = flip Y, {b} = flip FadeImage, {a} flip RefImage
};

uniform sampler2D MapDepthBuffer;
uniform sampler2D MapColorBuffer;
uniform sampler2D MapBlendFadeImage;
uniform sampler2D MapBlendReference;

#define ToneMapExposure ToneMap.r
#define ToneMapGamma    ToneMap.g
#define ToneMapFilmicHighlight ToneMap.b
#define ToneMapFilmicShadows   ToneMap.a
#define FadeImage		 CompareBlend.r
#define FadeSolid        CompareBlend.g
#define FadeReference    CompareBlend.b
#define ImageDifference  CompareBlend.a
#define FadeDissolve     FadeToColor.a
#define FlipX            Flips.x
#define FlipY			 Flips.y
#define FlipFadeImage    Flips.z
#define FlipReference    Flips.w

vec3 srcColor;
vec3 imgColor;
vec3 refColor;
float colorDifference;

float relativeLuminance(vec3 c) {
	return dot(vec3(0.2126, 0.7152, 0.0722), c);
}

vec3 filmicCompress(vec3 linearColor) {
	vec3 x = max(vec3(0.0), linearColor - vec3(0.004));
	return (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06);
}

vec3 filmicShadows(vec3 linearColor) {
	return linearColor;
}

vec3 calcToneMap(vec3 color, float exposure, float gamma, float highlights, float shadows) {
	float e = 2.5 * pow(2.0, exposure);
	color *= e;

	if (gamma > 0.0) {
		color = pow(color, vec3(1.0 / gamma));	
	}

	if (highlights > 0.0) {
		vec3 fcolor = filmicCompress(color);
		color = mix(color, fcolor, highlights);
	}

	if (shadows > 0.0) {
		vec3 fcolor = filmicShadows(color);
		color = mix(color, fcolor, shadows);
	}
	return color;
}

vec3 compareReference() {
	colorDifference = relativeLuminance(refColor) - relativeLuminance(srcColor);
	if (colorDifference < 0.0) return vec3(0.0, min(1.0, -colorDifference), 0.0);
	return vec3(min(1.0, colorDifference), 0.0, 0.0);
}

vec3 readTextures() {
	vec2 srcflip = vTexCoord.st;
	if (FlipX > 0.5) { srcflip.s = 1.0 - srcflip.s; }
	if (FlipY > 0.5) { srcflip.t = 1.0 - srcflip.t; }
	vec2 imgflip = FlipFadeImage > 0.5 ? vec2(srcflip.s, 1.0 - srcflip.t)
									   : srcflip.st;
	vec2 refflip = FlipReference > 0.5 ? vec2(srcflip.s, 1.0 - srcflip.t)
									   : srcflip.st;
	srcColor = texture(MapColorBuffer, srcflip).rgb;
	imgColor = texture(MapBlendFadeImage, imgflip).rgb;
	refColor = texture(MapBlendReference, refflip).rgb;
	return srcColor;
}

out vec4 oColor;

void main() {
	vec3 color = readTextures();
	if (FadeImage > 0.0) color = mix(color, imgColor, FadeImage);
	if (FadeSolid > 0.0) color = mix(color, FadeColor.rgb, FadeSolid);

	srcColor = calcToneMap(color, ToneMapExposure, ToneMapGamma, ToneMapFilmicHighlight, ToneMapFilmicShadows);	
	refColor = calcToneMap(refColor, ReferenceOps.r, ReferenceOps.g, ReferenceOps.b, ReferenceOps.a);		

	color = mix(srcColor, refColor, FadeReference);

	if (ImageDifference > 0.0) {
		color = mix(color, compareReference(), ImageDifference);
	}

	oColor = vec4(color, 1.0);
}