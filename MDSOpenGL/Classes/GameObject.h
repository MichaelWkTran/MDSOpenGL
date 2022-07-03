#pragma once
#include <glm/gtx/vector_angle.hpp>
#include <vector>
#include <thread>
std::thread;

class CGameManager;
class CGameObjectComponent;
class CCamera;

class CGameObject
{
private:
	bool m_bUpdateTransform;
	glm::vec3 m_v3Position;
	glm::fquat m_fquatRotation;
	glm::vec3 m_v3Scale;
	glm::mat4x4 m_mat4Model;

	bool m_bDeleteGameObject;

public:
	CGameManager* m_pGameManager;
	std::vector<CGameObjectComponent*> m_vComponents;
	
	CGameObject();
	~CGameObject() {};

	template <class T>
	T* AddComponent();

	const glm::mat4x4 GetModel() const;
	const glm::vec3 GetPosition() const; void SetPosition(const glm::vec3 _v3Position);
	const glm::fquat GetRotation() const; void SetRotation(const glm::fquat _fquatRotation);
	const glm::vec3 GetRotationEuler(bool _InDegrees = true) const; void SetRotationEuler(const glm::vec3 _v3Rotation, bool _InDegrees = true);
	const glm::vec3 GetScale() const; void SetScale(const glm::vec3 _v3Scale);

	const bool GetDeleteGameObject() const; void DeleteGameObject();
	const glm::vec3 Up() const;
	const glm::vec3 Forward() const;
	const glm::vec3 Right() const;

	void Start();
	void BeginUpdate();
	void Update();
	void EndUpdate();
	void Draw(const CCamera& _Camera);
};

template<class T>
inline T* CGameObject::AddComponent()
{
	m_vComponents.emplace_back((CGameObjectComponent*)new T);
	m_vComponents.back()->m_pGameObject = this;
	return (T*)m_vComponents.back();
}
