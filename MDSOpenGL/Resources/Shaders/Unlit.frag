#version 430 core

out vec4 FragColor;

in vec3 m_v3CurrentPosition;
in vec3 m_v3Normal;
in vec2 m_v2TextureCoord;
in vec3 m_v3Color;

//Texture Uniforms
#define DIFFUSE_NUMBER 1
uniform sampler2D uni_samp2DDiffuse[DIFFUSE_NUMBER];
#define SPECULAR_NUMBER 1
uniform sampler2D uni_samp2DSpecular[SPECULAR_NUMBER];

//Light Structs
struct stInfinitePointLight
{
	vec3 v3LightPosition;
	vec4 v4LightColour;
};

struct stPointLight
{
	vec3 v3LightPosition;
	vec4 v4LightColour;
	float fAttenuationExponent;
	float fAttenuationLinear;
	float fAttenuationConstant;
};

struct stDirectionalLight
{
	vec3 v3LightDirection;
	vec4 v4LightColour;
};

struct stSpotLight
{
	vec3 v3LightDirection;
	vec4 v4LightColour;
	float fOuterCone;
	float fInnerCone;
};

//Shared Light Uniforms
uniform vec3 uni_v3CameraPosition;
uniform vec4 uni_v4AmbientColour;

#define IP_LIGHT_NUMBER 1
uniform bool uni_bUsesInfinitePointLight = false;
uniform stInfinitePointLight uni_InfinitePointLight[IP_LIGHT_NUMBER];
#define P_LIGHT_NUMBER 1
uniform bool uni_bUsesPointLight = false;
uniform stPointLight uni_PointLight[P_LIGHT_NUMBER];
#define D_LIGHT_NUMBER 1
uniform bool uni_bUsesDirectionalLight = false;
uniform stDirectionalLight uni_DirectionalLight[D_LIGHT_NUMBER];
#define S_LIGHT_NUMBER 1
uniform bool uni_bUsesSpotLight = false;
uniform stSpotLight uni_SpotLight[S_LIGHT_NUMBER];

//Unique Light Uniforms
uniform float uni_fSpecularStrength = 0.3f;
uniform float uni_fShininess = 16.0f;

uniform float uni_fRimExponent = 16.0f;
uniform vec4 uni_v4RimColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);

//Global Variables
vec3 g_v3Normal;
vec3 g_v3Diffuse;
vec3 g_v3Specular;

void DiffuseColour(vec3 _v3LightColour, vec3 _v3LightDirection)
{
	g_v3Diffuse += _v3LightColour * max(dot(g_v3Normal, _v3LightDirection), 0.0f);
}

void SpecularColour(vec3 _v3LightDirection, float _fIntensity)
{
	if (_fIntensity <= 0) return;

	vec3 v3ViewDirection = normalize(uni_v3CameraPosition - m_v3CurrentPosition);
	vec3 v3HalfwayVector = normalize(-_v3LightDirection + v3ViewDirection);
	float fSpecularAmount = pow(max(dot(g_v3Normal, v3HalfwayVector), 0.0f), uni_fShininess);

	g_v3Specular += vec3(1.0f,1.0f,1.0f) * fSpecularAmount * uni_fSpecularStrength * _fIntensity;
}

vec3 RimColour()
{
	if (uni_v4RimColor.w <= 0) return vec3(0);

	float fRimFactor = 1.0f - dot(g_v3Normal, uni_v3CameraPosition - m_v3CurrentPosition);
	fRimFactor = smoothstep(0.0f, 1.0f, fRimFactor);
	fRimFactor = pow(fRimFactor, uni_fRimExponent);

	return fRimFactor * (uni_v4RimColor.xyz * uni_v4RimColor.w);
}

void InfinitePointLight(stInfinitePointLight _InfinitePointLight)
{
	vec3 v3LightPosition = _InfinitePointLight.v3LightPosition;
	vec4 v4LightColour = _InfinitePointLight.v4LightColour;
	
	DiffuseColour(v4LightColour.xyz, normalize(v3LightPosition - m_v3CurrentPosition));
	SpecularColour(v3LightPosition - m_v3CurrentPosition, v4LightColour.w);
}

void PointLight(stPointLight _PointLight)
{
	vec3 v3LightPosition = _PointLight.v3LightPosition;
	vec4 v4LightColour = _PointLight.v4LightColour;
	float fAttenuationExponent = _PointLight.fAttenuationExponent;
	float fAttenuationLinear = _PointLight.fAttenuationLinear;
	float fAttenuationConstant = _PointLight.fAttenuationConstant;

	vec3 v3LightVector = v3LightPosition - m_v3CurrentPosition;
	float fLightDistance = length(v3LightVector);
	float fIntensity = v4LightColour.w / ((fAttenuationExponent * fLightDistance * fLightDistance) + (fAttenuationLinear * fLightDistance) + fAttenuationConstant);
	
	DiffuseColour(v4LightColour.xyz, normalize(v3LightVector));
	SpecularColour(v3LightPosition - m_v3CurrentPosition, fIntensity);
}

void DirectionalLight(stDirectionalLight _DirectionalLight)
{
	vec3 v3LightDirection = -normalize(_DirectionalLight.v3LightDirection);
	vec4 v4LightColour = _DirectionalLight.v4LightColour;

	DiffuseColour(v4LightColour.xyz, v3LightDirection);
	SpecularColour(v3LightDirection, v4LightColour.w);
}

void SpotLight(stSpotLight _SpotLight)
{
	vec3 v3LightDirection = normalize(_SpotLight.v3LightDirection);
	vec4 v4LightColour = _SpotLight.v4LightColour;
	float fOuterCone = _SpotLight.fOuterCone;
	float fInnerCone = _SpotLight.fInnerCone;

	float fAngle = dot(vec3(0.0f, -1.0f, 0.0f), -v3LightDirection);
	float fIntensity = v4LightColour.w * clamp((fAngle - fOuterCone) / (fInnerCone - fOuterCone), 0.0f, 1.0f);

	DiffuseColour(v4LightColour.xyz, v3LightDirection);
	SpecularColour(v3LightDirection, fIntensity);
}

void main()
{
	g_v3Normal = normalize(m_v3Normal);

	//Get Texture Colours
	vec4 v4DiffuseTexture = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < DIFFUSE_NUMBER; i++) v4DiffuseTexture *= texture(uni_samp2DDiffuse[i], m_v2TextureCoord);
	v4DiffuseTexture *= vec4(m_v3Color, 1.0f);
	
	vec4 v4SpecularTexture = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < SPECULAR_NUMBER; i++) v4SpecularTexture *= texture(uni_samp2DSpecular[i], m_v2TextureCoord);

	//Calculate Diffuse and Specular Colours	
	for (int i = 0; i < IP_LIGHT_NUMBER && uni_bUsesInfinitePointLight; i++) InfinitePointLight(uni_InfinitePointLight[i]);

	for (int i = 0; i < P_LIGHT_NUMBER && uni_bUsesPointLight; i++) PointLight(uni_PointLight[i]);

	for (int i = 0; i < D_LIGHT_NUMBER && uni_bUsesDirectionalLight; i++) DirectionalLight(uni_DirectionalLight[i]);

	for (int i = 0; i < S_LIGHT_NUMBER && uni_bUsesSpotLight; i++) SpotLight(uni_SpotLight[i]);

	//Combine Calculated Colours
	FragColor = vec4
	(
		(v4DiffuseTexture.xyz * (g_v3Diffuse + (uni_v4AmbientColour.xyz * uni_v4AmbientColour.w))) + 
		(v4SpecularTexture.xyz * g_v3Specular) + 
		RimColour(),
		1.0f
	);
}