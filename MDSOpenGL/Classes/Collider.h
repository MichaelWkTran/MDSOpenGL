#pragma once
#include "UpdatedObject.h"
#include <glm/glm.hpp>
#include <set>

class CTransform;

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
	std::set<CCollider*> m_CurrentlyCollidingWith;
	bool m_bTrigger;
	bool m_bStatic;
	CTransform* m_pTransform;

	CCollider()
	{
		m_bTrigger = false;
		m_bStatic = false;
		m_pTransform = nullptr;
	}
	~CCollider() {}
	virtual const stCollisionPoints CollisionMethod(CCollider* _pOther) = 0;
	void CheckCollision(std::set<CCollider*> _CollidersInWorld);
};

namespace CollisionMethodAlgorithms
{
	//https://github.com/IainWinter/IwEngine/blob/tutorial/IwEngine/src/physics/Collision/algo/ManifoldFactory.cpp
	const stCollisionPoints SphereSphereCollision(CCollider* _pA, CCollider* _pB);
	const stCollisionPoints SpherePlaneCollision(CCollider* _pA, CCollider* _pB);
	const stCollisionPoints SphereCapsuleCollision(CCollider* _pA, CCollider* _pB);
}