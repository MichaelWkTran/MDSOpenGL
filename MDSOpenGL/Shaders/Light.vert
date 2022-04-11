#version 330 core

layout (location = 0) in vec3 in_v3Position;

uniform mat4 uni_mat4Model;
uniform mat4 uni_mat4CameraMatrix;

void main()
{
	gl_Position = uni_mat4CameraMatrix * uni_mat4Model * vec4(in_v3Position, 1.0f);
}