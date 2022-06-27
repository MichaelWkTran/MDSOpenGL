#version 430 core

layout (location = 0) in vec3 in_v3Position;
layout (location = 1) in vec3 in_v3Normal;
layout (location = 2) in vec2 in_v2TextureCoord;
layout (location = 3) in vec3 in_v3Color;

out vec3 m_v3Position;
out vec3 m_v3Normal;
out vec2 m_v2TextureCoord;
out vec3 m_v3Color;

//Mesh Uniforms
uniform mat4 uni_mat4CameraMatrix;
uniform mat4 uni_mat4Model;

void main()
{
	m_v3Position = vec3(uni_mat4Model * vec4(in_v3Position, 1.0f));
	m_v3Normal = normalize(mat3(transpose(inverse(uni_mat4Model))) * in_v3Normal);
	m_v2TextureCoord = in_v2TextureCoord;
	m_v3Color = in_v3Color;

	gl_Position = uni_mat4CameraMatrix * vec4(m_v3Position, 1.0);
}