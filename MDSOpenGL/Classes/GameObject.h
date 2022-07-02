#include "Mesh.h"
#include "GameObjectComponent.h"
#include <glm/gtx/vector_angle.hpp>
#include <vector>

class CGameObject
{
private:
	glm::mat4x4 m_mat4Model;

	bool m_bUpdateTransform;
	glm::vec3 m_v3Position;
	glm::fquat m_fquatRotation;
	glm::vec3 m_v3Scale;

public:
	std::vector<CGameObjectComponent*> m_Components;
	CMesh<> m_Mesh;
	
	CGameObject();
	~CGameObject() {};

	const glm::mat4x4 GetModel() const;
	const glm::vec3 GetPosition() const; void SetPosition(const glm::vec3 _v3Position);
	const glm::fquat GetRotation() const; void SetRotation(const glm::fquat _fquatRotation);
	const glm::vec3 GetScale() const; void SetScale(const glm::vec3 _v3Scale);

	void Draw(const CCamera& _Camera);
};