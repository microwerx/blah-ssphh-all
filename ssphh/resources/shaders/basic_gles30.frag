#version 300 es
precision highp float;

#define MAX_DIRTOLIGHT_COUNT	8
#define MAX_POINTLIGHT_COUNT	8
#define MAX_MATERIAL_COUNT		32

//////////////////////////////////////////////////////////////////////
// MATERIAL UNIFORM BLOCK ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

struct Material {
	vec4 Kd;
	vec4 Ks;
	vec4 Ke;
	vec4 Kdroughness;
	vec4 Ksroughness;
	vec4 Kior;
	vec4 KmetallicSpecular;
	vec4 KclearcoatSheen;
};

layout(std140) uniform MaterialBlock {
	Material materials[MAX_MATERIAL_COUNT];
};

//////////////////////////////////////////////////////////////////////
// DIRTO LIGHT UNIFORM BLOCK /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

struct DirToLight {
	vec4 dirTo;
	vec4 shadow;
	vec4 shape;
	vec4 E0;
};

layout(std140) uniform DirToLightBlock {
	DirToLight DirtoLights[MAX_DIRTOLIGHT_COUNT];
};

uniform sampler2D DirToLightColorMaps[MAX_DIRTOLIGHT_COUNT];
uniform sampler2D DirToLightDepthMaps[MAX_DIRTOLIGHT_COUNT];

//////////////////////////////////////////////////////////////////////
// POINT LIGHT UNIFORM BLOCK /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

struct PointLight {
	vec3 position;
	vec3 E0;
	float falloffRadius;
};

layout(std140) uniform PointLightBlock {
	PointLight pointLights[MAX_POINTLIGHT_COUNT];
};

uniform samplerCube PointLightColorMaps[MAX_POINTLIGHT_COUNT];
uniform samplerCube PointLightDepthMaps[MAX_POINTLIGHT_COUNT];

//////////////////////////////////////////////////////////////////////
// SAMPLER UNIFORMS //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

uniform sampler2D MapKd;
uniform sampler2D MapKs;
uniform int MtlID;

uniform sampler2D MapTest2D;

//////////////////////////////////////////////////////////////////////
// VERTEX SHADER INPUTS //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vCameraPosition;
in vec3 vColor;
in vec4 vShadowCoords[MAX_DIRTOLIGHT_COUNT];

//////////////////////////////////////////////////////////////////////
// FRAGMENT SHADER OUTPUTS ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

out vec4 oColor;

vec3 decodeSRGB(vec3 color) {
	return pow(color, vec3(2.2));
}

vec3 ReadMapKd() {
	return decodeSRGB(texture(MapKd, vTexCoord).rgb);
}

bool sameHemisphere(float NdotL, float NdotV) {
	return (NdotL * NdotV) > 0.0;
}

// N, L, and V need to be normalized
vec3 calcPBR(vec3 N, vec3 L, vec3 V) {
	float NdotL = dot(N, L);
	float NdotV = dot(N, V);
	float lighting = 0.1;
	// if (!sameHemisphere(NdotL, NdotV)) {
	// 	return vec3(0.1);
	// }
	return vec3(0.1 + 0.9 * max(0.0, NdotL));
}


//////////////////////////////////////////////////////////////////////
// DIRTO SHADOW MAPS /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


// calcShadow returns 0 if completely shadowed, 1 if not shadowed
float calcShadow(int mapId) {
	float texel = 1.0;
	switch(mapId) {
	case 0:
		texel = textureProj(DirToLightDepthMaps[0], vShadowCoords[0].xyw).r;
		break;
	case 1:
		texel = textureProj(DirToLightDepthMaps[1], vShadowCoords[1].xyw).r;
		break;
	case 2:
		texel = textureProj(DirToLightDepthMaps[2], vShadowCoords[2].xyw).r;
		break;
	}
	return texel;
}


//////////////////////////////////////////////////////////////////////
// MAIN FUNCTION /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void main() {
	int light = 0;
	float shadow = calcShadow(0);
	vec4 texel = textureProj(MapTest2D, vShadowCoords[0].xyw);
	if (shadow == 1.0)
		texel = vec4(1.0, 1.0, 0.0, 1.0);
	if (shadow == 0.0)
		texel = vec4(0.0, 1.0, 1.0, 1.0);
	oColor = vec4(texel.rgb, 1.0);
	return;

	vec3 color;
	if (length(vColor) > 0.0) {
		oColor = vec4(vColor, 1.0);
		return;
	}

	vec3 N = normalize(vNormal);
	vec3 V = normalize(vPosition - vCameraPosition);

	vec3 lighting = vec3(0.0);
	for (int i = 0; i < MAX_DIRTOLIGHT_COUNT; i++) {
		vec3 L = normalize(DirtoLights[i].dirTo.xyz);
		if (L.y <= 0.0) continue;
		float shadow = calcShadow(i);
		vec4 texel = textureProj(MapTest2D, vShadowCoords[1].xyw);
		if (shadow == 1.0)
			texel = vec4(1.0, 1.0, 0.0, 1.0);
		lighting += texel.rgb;
		if (shadow == 0.0) discard;
		//lighting += shadow;
		vec3 E0 = DirtoLights[i].E0.rgb;
		lighting += E0 * max(0.0, L.y) * calcPBR(N, L, V);
	}

	vec3 mtlkd = materials[MtlID].Kd.rgb;
	float kdmix = materials[MtlID].Kd.a;
	vec3 kd = mix(mtlkd, ReadMapKd(), kdmix);
	if (length(mtlkd) < 0.1)
		color = 0.5 * vNormal + 0.5 + 0.5 * kd + pointLights[0].E0;
	else {
		color = vec3(float(MtlID) / 32.0, mtlkd.gb);
	}
	oColor = vec4(lighting * kd, 1.0);
}
