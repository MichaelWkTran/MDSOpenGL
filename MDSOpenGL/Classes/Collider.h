#pragma once
#include "UpdatedObject.h"
#include <glm/glm.hpp>
#include <set>

class CTransform;
class CRigidBody;

//https://www.youtube.com/watch?v=-_IspRG548E
struct stCollisionPoints
{
	bool bHaveCollision;
	glm::vec3 A; //Furthest point of A into B
	glm::vec3 B; //Furthest point of B into A
};

class CCollider
{
public:
	static std::set<CCollider*> m_setCollidersInWorld;

	std::set<CCollider*> m_CurrentlyCollidingWith;
	bool m_bTrigger;
	CTransform* m_pTransform;
	CRigidBody* m_pRigidbody;

	CCollider()
	{
		m_setCollidersInWorld.insert(this);
		m_bTrigger = false;
		m_pTransform = nullptr;
		m_pRigidbody = nullptr;
	}
	~CCollider()
	{
		m_setCollidersInWorld.erase(this);
	}

	void ConstructComponent(CTransform* _pTransform, CRigidBody* _pRigidbody);
	virtual const stCollisionPoints CollisionMethod(CCollider* _pOther) = 0;
	void CheckCollision();
};

namespace CollisionMethodAlgorithms
{
	//https://github.com/IainWinter/IwEngine/blob/tutorial/IwEngine/src/physics/Collision/algo/ManifoldFactory.cpp
	const stCollisionPoints SphereSphereCollision(CCollider* _pA, CCollider* _pB);
	//const stCollisionPoints SpherePlaneCollision(CCollider* _pA, CCollider* _pB);
	//const stCollisionPoints SphereCapsuleCollision(CCollider* _pA, CCollider* _pB);
}