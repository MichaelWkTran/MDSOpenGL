#version 330 core

out vec4 FragColor;

in vec3 m_v3CurrentPosition;
in vec3 m_v3Normal;
in vec3 m_v3Color;
in vec2 m_v2TextureCoord;

uniform sampler2D uni_samp2DDiffuse0;
uniform sampler2D uni_samp2DSpecular0;
uniform vec4 uni_v4LightColor;
uniform vec3 uni_v3LightPosition;
uniform vec3 uni_v3CameraPosition;

vec4 PointLight(float _fAmbient)
{
	vec3 v3LightVec = uni_v3LightPosition - m_v3CurrentPosition;
	float fLightDistance = length(v3LightVec);
	float fA = 3.0f;
	float fB = 0.7f;
	float fIntensity = 1.0f / ((fA * fLightDistance * fLightDistance) + (fB * fLightDistance) + 1.0f);

	vec3 v3Normal = normalize(m_v3Normal);
	vec3 v3lightDirection = normalize(uni_v3LightPosition - m_v3CurrentPosition);
	float fDiffuse = max(dot(v3Normal, v3lightDirection), 0.0f);

	float fSpecularLight = 0.5f;
	vec3 v3ViewDirection = normalize(uni_v3CameraPosition - m_v3CurrentPosition);
	vec3 v3ReflectionDirection = reflect(-v3lightDirection, v3Normal);
	float fSpecAmount = pow(max(dot(v3ViewDirection, v3ReflectionDirection), 0.0f), 16);
	float fSpecular = fSpecAmount * fSpecularLight;

	return (texture(uni_samp2DDiffuse0, m_v2TextureCoord) * m_v3Color * uni_v4LightColor * ((fDiffuse  * fIntensity) + _fAmbient)) + 
	       (texture(uni_samp2DSpecular0, m_v2TextureCoord).r * fSpecular * fIntensity);
}

vec4 DirectionalLight(float _fAmbient)
{
	vec3 v3Normal = normalize(m_v3Normal);
	vec3 v3lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float fDiffuse = max(dot(v3Normal, v3lightDirection), 0.0f);

	float fSpecularLight = 0.5f;
	vec3 v3ViewDirection = normalize(uni_v3CameraPosition - m_v3CurrentPosition);
	vec3 v3ReflectionDirection = reflect(-v3lightDirection, v3Normal);
	float fSpecAmount = pow(max(dot(v3ViewDirection, v3ReflectionDirection), 0.0f), 16);
	float fSpecular = fSpecAmount * fSpecularLight;

	return (texture(uni_samp2DDiffuse0, m_v2TextureCoord) * m_v3Color * uni_v4LightColor * (fDiffuse + _fAmbient)) + 
	       (texture(uni_samp2DSpecular0, m_v2TextureCoord).r * fSpecular);
}

vec4 SpotLight(float _fAmbient)
{
	float fOuterCone = 0.9f;
	float fInnerCone = 0.95f;

	vec3 v3Normal = normalize(m_v3Normal);
	vec3 v3lightDirection = normalize(uni_v3LightPosition - m_v3CurrentPosition);
	float fDiffuse = max(dot(v3Normal, v3lightDirection), 0.0f);

	float fSpecularLight = 0.5f;
	vec3 v3ViewDirection = normalize(uni_v3CameraPosition - m_v3CurrentPosition);
	vec3 v3ReflectionDirection = reflect(-v3lightDirection, v3Normal);
	float fSpecAmount = pow(max(dot(v3ViewDirection, v3ReflectionDirection), 0.0f), 16);
	float fSpecular = fSpecAmount * fSpecularLight;

	float fAngle = dot(vec3(0.0f, -1.0f, 0.0f), -v3lightDirection);
	float fIntensity = clamp((fAngle - fOuterCone) / (fInnerCone - fOuterCone), 0.0f, 1.0f);

	return (texture(uni_samp2DDiffuse0, m_v2TextureCoord) * m_v3Color * uni_v4LightColor * ((fDiffuse  * fIntensity) + _fAmbient)) + 
	       (texture(uni_samp2DSpecular0, m_v2TextureCoord).r * fSpecular * fIntensity);
}

void main()
{
	float fAmbient = 0.2f;

	FragColor = SpotLight(fAmbient);
}