#version 100

// Input from OpenGL ES
uniform mat4 CameraMatrix;
uniform mat4 ProjectionCameraMatrix;
uniform mat4 WorldMatrix;

// Input from OpenGL ES
attribute vec4 aPosition;

void main()
{
	gl_Position = ProjectionCameraMatrix * WorldMatrix * aPosition;
}