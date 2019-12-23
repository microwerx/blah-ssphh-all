#version 100
#ifdef GL_ES
precision highp float;
#endif

attribute vec4 aPosition;
attribute vec3 aTexCoord;

varying vec3 vTexCoord;

void main()
{
	vTexCoord = aTexCoord;
	gl_Position = vec4(aPosition.xyz, 1.0);
}
