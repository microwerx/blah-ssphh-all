#version 100
#ifdef GL_ES
precision highp float;
#endif

// Input from OpenGL ES
uniform mat4 CameraMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 WorldMatrix;
uniform mat4 ModelViewProjectionMatrix;

// Input from OpenGL ES
attribute vec4 aPosition;
attribute vec4 aColor;
attribute vec3 aTexCoord;
attribute vec3 aNormal;

// Output to Fragment Shader
varying vec4 vColor;
varying vec3 vNormal;
varying vec3 vTexCoord;

void main()
{
        vColor = aColor;
        vTexCoord = aTexCoord;
        vNormal = mat3(WorldMatrix) * aNormal;
        gl_Position = (ProjectionMatrix * CameraMatrix * WorldMatrix * aPosition);
}
