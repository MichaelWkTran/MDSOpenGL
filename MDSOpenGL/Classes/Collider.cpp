#include "Collider.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "CapsuleCollider.h"
#include "RigidBody.h"
#include <iostream>

std::set<CCollider*> CCollider::m_setCollidersInWorld;

void CCollider::CheckCollision()
{
	//Clear m_mapCurrentlyCollidingWith
	m_mapCurrentlyCollidingWith.clear();

	//Loop through all colliders in the game and check collisions
	for (auto& pCollider : m_setCollidersInWorld)
	{
		//Ignore checking collisions with itself
		if (pCollider == this) continue;

		//Check whether the colliding with set already contains the collider
		if (m_mapCurrentlyCollidingWith.find(pCollider) != m_mapCurrentlyCollidingWith.end()) continue;

		//Get collision data
		stCollisionPoints CollisionPoints = CollisionMethod(pCollider);

		//Check whether the colliders are colliding
		if (!CollisionPoints.bHaveCollision) continue;

		//Add each others colliders to their sets
		m_mapCurrentlyCollidingWith.insert(std::make_pair<>(pCollider, CollisionPoints));
		pCollider->m_mapCurrentlyCollidingWith.insert(std::make_pair<>(pCollider, CollisionPoints.Swap()));
		
		////Skip collider separation if it is a trigger
		//if (m_bTrigger) continue;
		//
		////Collider separation
		//glm::vec3 v3SeparationNormal = glm::normalize(CollisionPoints.A - CollisionPoints.B);
		//float fSepparationDistance = glm::distance(CollisionPoints.A, CollisionPoints.B);
		//
		////If both colliders have a rigidbody
		//bool bDynamic = false;
		//if (m_pRigidbody)
		//	if (!m_pRigidbody->m_bIsKinematic) bDynamic = true;
		//
		//bool bOtherDynamic = false;
		//if (pCollider->m_pRigidbody)
		//	if (!pCollider->m_pRigidbody->m_bIsKinematic) bOtherDynamic = true;
		//
		/////////////////
		//if (bDynamic && bOtherDynamic)
		//{
		//	m_pRigidbody->m_v3Velocity += v3SeparationNormal * fSepparationDistance * 0.5f;
		//	pCollider->m_pRigidbody->m_v3Velocity += -v3SeparationNormal * fSepparationDistance * 0.5f;
		//}
		//else if (bDynamic && !bOtherDynamic)
		//{
		//	m_pRigidbody->m_v3Velocity += v3SeparationNormal * fSepparationDistance;
		//}
		//else if (!bDynamic && bOtherDynamic)
		//{
		//	m_pRigidbody->m_v3Velocity += -v3SeparationNormal * fSepparationDistance;
		//}
	}

	//Temporary constraint for spheres collide with ground
	if (m_pTransform == nullptr) return;

	CSphereCollider* pSphere = dynamic_cast<CSphereCollider*>(this);
	if (pSphere && m_pTransform->GetPosition().y - pSphere->m_fRadius < 0)
	{
		glm::vec3 v3Position = m_pTransform->GetPosition();
		v3Position.y = pSphere->m_fRadius;
		m_pTransform->SetPosition(v3Position);
	}
}

const stCollisionPoints CollisionMethodAlgorithms::SphereSphereCollision(CCollider* _pA, CCollider* _pB)
{
	CSphereCollider* pA = dynamic_cast<CSphereCollider*>(_pA);
	CSphereCollider* pB = dynamic_cast<CSphereCollider*>(_pB);
	if (pA == nullptr || pB == nullptr) return stCollisionPoints();
	
	//Get vector between one collider to the other
	glm::vec3 v3AtoB = pB->m_pTransform->GetPosition() - pA->m_pTransform->GetPosition();
	
	//Check whether the spheres are too far from each other to be colliding
	if (glm::length(v3AtoB) > pA->m_fRadius + pB->m_fRadius) return stCollisionPoints();

	//Calculate stCollisionPoints
	glm::vec3 v3AtoBNormal = glm::normalize(v3AtoB);

	return stCollisionPoints
	{
		true,
		pA->m_pTransform->GetPosition() + (v3AtoBNormal * pA->m_fRadius),
		pB->m_pTransform->GetPosition() + (-v3AtoBNormal * pB->m_fRadius)
	};

	//return stCollisionPoints
	//{
	//	true,
	//	pA->m_pTransform->GetPosition() + (v3AtoB / 2.0f),
	//	pB->m_pTransform->GetPosition() + (-v3AtoB / 2.0f)
	//};
}

//const stCollisionPoints CollisionMethodAlgorithms::SpherePlaneCollision(CCollider* _pA, CCollider* _pB)
//{
//	CSphereCollider* pA = dynamic_cast<CSphereCollider*>(_pA);
//	CPlaneCollider* pB = dynamic_cast<CPlaneCollider*>(_pB);
//	if (pA == nullptr || pB == nullptr) return stCollisionPoints();
//
//	//Translate the sphere collider to plane space
//	//glm::vec3 v3APosition = pA->m_pTransform->GetPosition();
//	//v3APosition -= pB->m_pTransform->GetPosition();
//	//v3APosition *= glm::inverse(pB->m_pTransform->GetRotation());
//
//	//Check whether the sphere would collide with the plane
//
//
//	//Calculate stCollisionPoints
//
//	
//	glm::vec3 v3APosition = _pA->m_pTransform->GetPosition();
//	glm::vec3 v3BNormal = _pB->m_pTransform->Up();
//	
//	glm::vec3 P = v3BNormal * b->Plane.D;
//	float d = glm::dot(v3APosition - P, v3BNormal);
//	
//	//If the sphere is too far from the plane
//	if (d > pA->m_fRadius)
//	{
//		return stCollisionPoints();
//	}
//	
//	return stCollisionPoints
//	{
//		true,
//		v3APosition - (v3BNormal * d),
//		v3APosition - (v3BNormal * pA->m_fRadius)
//	};
//}

//const stCollisionPoints CollisionMethodAlgorithms::SphereCapsuleCollision(CCollider* _pA, CCollider* _pB)
//{
//	float Bhs = 1.0f;
//	float Brs = 1.0f;
//
//	vector3 s = tb->WorldScale();
//	if (b->Direction == vector3::unit_x) {
//		Bhs = s.x;
//		Brs = vector2(s.y, s.z).major();
//	}
//
//	else if (b->Direction == vector3::unit_y) {
//		Bhs = s.y;
//		Brs = vector2(s.x, s.z).major();
//	}
//
//	else if (b->Direction == vector3::unit_z) {
//		Bhs = s.z;
//		Brs = vector2(s.x, s.y).major();
//	}
//
//	vector3 offset = b->Direction * tb->WorldRotation() * (b->Height * Bhs / 2 - b->Radius * Brs);
//
//	vector3 A = a->Center + ta->WorldPosition();
//	vector3 B = b->Center - offset + tb->WorldPosition();
//	vector3 C = b->Center + offset + tb->WorldPosition(); // might not be correct
//
//	float Ar = a->Radius * ta->WorldScale().major();
//	float Br = b->Radius * Brs;
//
//	vector3 BtoA = A - B;
//	vector3 BtoC = C - B;
//
//	float   d = iw::clamp(BtoC.normalized().dot(BtoA), 0.0f, BtoC.length());
//	vector3 D = B + BtoC.normalized() * d;
//
//	vector3 AtoD = D - A;
//	vector3 DtoA = A - D;
//
//	if (AtoD.length() > Ar + Br) {
//		return {
//			0, 0,
//			0,
//			0,
//			false
//		};
//	}
//
//	A += AtoD.normalized() * Ar;
//	D += DtoA.normalized() * Br;
//
//	AtoD = D - A;
//
//	return {
//		A, D,
//		AtoD.normalized(),
//		AtoD.length(),
//		true
//	};
//}
