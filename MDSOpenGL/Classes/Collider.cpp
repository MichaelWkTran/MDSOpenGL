#include "Collider.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "CapsuleCollider.h"

void CCollider::CheckCollision(std::set<CCollider*> _CollidersInWorld)
{
	for (auto pCollider : _CollidersInWorld)
	{
		//Ignore checking collisions with itself
		if (pCollider == this) continue;

		//Check whether the colliding with set already contains the collider
		if (m_CurrentlyCollidingWith.find(pCollider) != m_CurrentlyCollidingWith.end()) continue;

		//Get collision data
		stCollisionPoints CollisionPoints = CollisionMethod(pCollider);

		//Check whether the colliders are colliding
		if (!CollisionPoints.bHaveCollision)
		{
			m_CurrentlyCollidingWith.erase(pCollider);
			continue;
		}

		//Add each others colliders to their sets
		m_CurrentlyCollidingWith.insert(pCollider);
		pCollider->m_CurrentlyCollidingWith.insert(this);

		//Skip collider separation if it is a trigger
		if (m_bTrigger) continue;

		//Collider separation
		glm::vec3 v3SeparationNormal = glm::normalize(CollisionPoints.A - CollisionPoints.B);
		float fSepparationDistance = glm::distance(CollisionPoints.A, CollisionPoints.B);

		if (!m_bStatic && !pCollider->m_bStatic)
		{
			m_pTransform->SetPosition
			(
				m_pTransform->GetPosition() + 
				(v3SeparationNormal * fSepparationDistance * 0.5f)
			);
			
			pCollider->m_pTransform->SetPosition
			(
				pCollider->m_pTransform->GetPosition() +
				(-v3SeparationNormal * fSepparationDistance * 0.5f)
			);
		}
		else if (!m_bStatic && pCollider->m_bStatic)
		{
			m_pTransform->SetPosition
			(
				m_pTransform->GetPosition() +
				(v3SeparationNormal * fSepparationDistance)
			);
		}
		else if (m_bStatic && !pCollider->m_bStatic)
		{
			pCollider->m_pTransform->SetPosition
			(
				pCollider->m_pTransform->GetPosition() +
				(-v3SeparationNormal * fSepparationDistance)
			);
		}
	}
}

const stCollisionPoints CollisionMethodAlgorithms::SphereSphereCollision(CCollider* _pA, CCollider* _pB)
{
	CSphereCollider* pA = dynamic_cast<CSphereCollider*>(_pA);
	CSphereCollider* pB = dynamic_cast<CSphereCollider*>(_pB);
	if (pA == nullptr || pB == nullptr) return stCollisionPoints();
	
	//Get vector between one collider to the other
	glm::vec3 v3AtoB = pA->m_pTransform->GetPosition();
	
	//Check whether the spheres are too far from each other to be colliding
	if (v3AtoB.length() > pA->m_fRadius + pB->m_fRadius) return stCollisionPoints();

	//Calculate stCollisionPoints
	glm::vec3 v3AtoBNormal = glm::normalize(v3AtoB);

	return stCollisionPoints
	{
		true,
		pA->m_pTransform->GetPosition() + (v3AtoBNormal * pA->m_fRadius),
		pB->m_pTransform->GetPosition() + (-v3AtoBNormal * pB->m_fRadius)
	};
}

//const stCollisionPoints CollisionMethodAlgorithms::SpherePlaneCollision(CCollider* _pA, CCollider* _pB)
//{
//	CSphereCollider* pA = dynamic_cast<CSphereCollider*>(_pA);
//	CPlaneCollider* pB = dynamic_cast<CPlaneCollider*>(_pB);
//	if (pA == nullptr || pB == nullptr) return stCollisionPoints();
//
//	//
//	glm::vec3 A = _pA->m_pTransform->GetPosition();
//	glm::vec3 N = _pB->m_pTransform->Up();
//
//	glm::vec3 P = N * b->Plane.D + tb->WorldPosition();
//
//	float Ar = a->Radius * ta->WorldScale().x;
//
//	float d = glm::dot(A - P);
//
//	if (d > Ar) {
//		return {
//			0, 0,
//			0,
//			0,
//			false
//		};
//	}
//
//	vector3 B = A - N * d;
//	A = A - N * Ar;
//
//	return {
//		A, B,
//		N.normalized(),
//		(B - A).length(),
//		true
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
