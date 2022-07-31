#pragma once
#include "UpdatedObject.h"
#include <glm/glm.hpp>
#include <set>
#include <map>

class CTransform;
class CRigidBody;

//https://www.youtube.com/watch?v=-_IspRG548E
struct stCollisionPoints
{
	bool bHaveCollision;
	glm::vec3 A; //Furthest point of A into B
	glm::vec3 B; //Furthest point of B into A

	stCollisionPoints Swap()
	{
		return stCollisionPoints{ bHaveCollision, B, A };
	}
};

/*CCollider Provides collision data but does not change the transform of the object*/
class CCollider
{
protected:
	CTransform* m_pTransform;

public:
	static std::set<CCollider*> m_setCollidersInWorld;

	std::map<CCollider*, stCollisionPoints> m_mapCurrentlyCollidingWith;
	
	CCollider()
	{
		m_setCollidersInWorld.insert(this);
		m_pTransform = nullptr;
	}
	~CCollider()
	{
		m_setCollidersInWorld.erase(this);
	}

	void ConstructComponent(CTransform& _Transform)
	{
		m_pTransform = &_Transform;
	}
	//What CollisionMethodAlgorithms should this collider use
	virtual const stCollisionPoints CollisionMethod(CCollider* _pOther) = 0;
	//Check collisions between this collider any all other coliders in the world
	void CheckCollision();

	CTransform* GetTransform() const
	{
		return m_pTransform;
	}
};

namespace CollisionMethodAlgorithms
{
	//https://github.com/IainWinter/IwEngine/blob/tutorial/IwEngine/src/physics/Collision/algo/ManifoldFactory.cpp
	const stCollisionPoints SphereSphereCollision(CCollider* _pA, CCollider* _pB);
	//const stCollisionPoints SpherePlaneCollision(CCollider* _pA, CCollider* _pB);
	//const stCollisionPoints SphereCapsuleCollision(CCollider* _pA, CCollider* _pB);
}