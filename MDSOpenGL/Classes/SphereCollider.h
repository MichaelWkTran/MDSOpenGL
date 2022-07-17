#pragma once
#include "Collider.h"

class CSphereCollider : public CCollider
{
public:
	float m_fRadius;

	CSphereCollider() { m_fRadius = 0.5f; }
	~CSphereCollider() {};
	virtual const stCollisionPoints CollisionMethod(CCollider* _pOther) override;
};