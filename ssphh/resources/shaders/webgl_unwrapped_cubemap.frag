#version 100
#ifdef GL_ES
precision highp float;
#endif

// Input from OpenGL ES
uniform samplerCube uCubeTexture;

// Input from Vertex Shader
varying vec4 vColor;
varying vec3 vTexCoord;

void main()
{
	vec4 outputColor = textureCube(uCubeTexture, vTexCoord);
	gl_FragColor = outputColor;//vec4((0.5 * fsTexCoord) + 0.5, 1.0);//outputColor;
}
