#version 300 es
precision highp float;

layout(location = 0) in vec4 aPosition;
layout(location = 2) in vec3 aTexCoord;

out vec3 vTexCoord;

void main()
{
	vTexCoord = aTexCoord;
	gl_Position = vec4(aPosition.xyz, 1.0);
}
