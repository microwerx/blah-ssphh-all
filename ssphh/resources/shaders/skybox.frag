#version 100
#ifdef GL_ES
precision highp float;
#endif

// Input from OpenGL ES
uniform samplerCube MapEnviroCube;
uniform samplerCube uHosekWilkieTexture;
uniform int uIsHosekWilkie;
uniform int uIsHemisphere;
uniform float ToneMapScale;
uniform float ToneMapGamma;
uniform float ToneMapExposure;

uniform vec3 SunDirTo;	// Direction of the light from the origin of the scene
uniform vec4 SunE0;		// Sun Disk Radiance - vec4(vec3(SunE0), SunMonoE0)
uniform vec3 MoonDirTo; // Direction of the light from the origin of the scene
uniform vec4 MoonE0;	// Moon Disk Radiance - vec4(vec4(MoonE0), MoonMonoE0)
uniform vec4 GroundE0;  // Ground Radiance from the reflected sun.

// Input from Vertex Shader
varying vec4 vColor;
varying vec3 vTexCoord;
varying vec3 vNormal;

vec4 GetPBSky(vec3 L, float spread)
{
	bool inDisc = false;
	bool atNight = false;

	if (SunDirTo.y >= 0.0)
	{
		atNight = false;
	}
	else
	{
		atNight = true;
	}

	float cosine = max(0.0, dot(normalize(L), normalize(SunDirTo)));
	if (cosine > 0.99995)
	{
		inDisc = true;
	} 

	vec4 outputColor = vec4(0.0);
	if (atNight)
	{	
		outputColor = vec4(0.0);
	}
	else
	{
		if (L.y >= 0.0)
		{
			if (inDisc)
			{
				outputColor = SunE0;
			}
			else
			{
				outputColor = textureCube(MapEnviroCube, L);
			}
		}
		else
		{
			outputColor = textureCube(MapEnviroCube, L);
		}
	}
	return outputColor;
}

void main()
{	
	//vec3 finalColor = GetPBSky(vTexCoord, 0.0).rgb * ToneMapExposure;
	vec3 finalColor = textureCube(MapEnviroCube, vTexCoord).rgb;

	// exposure and gamma
	// if (ToneMapGamma > 0.0)
	// 	finalColor = pow(finalColor, vec3(1.0 / ToneMapGamma));	

    gl_FragColor = vec4(vTexCoord + finalColor, 1.0);
}
