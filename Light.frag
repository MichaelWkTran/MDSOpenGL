#version 330 core

out vec4 m_v4FragColor;

uniform vec4 uni_v4LightColor;

void main()
{
	m_v4FragColor = uni_v4LightColor;
}