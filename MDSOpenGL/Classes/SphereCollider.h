#pragma once
#include "Collider.h"

class CSphereCollider : public CCollider
{
public:
	float m_fRadius;

	CSphereCollider(float _fRadius)
	{
		m_fRadius = _fRadius;
	}
	~CSphereCollider() {};
	const stCollisionPoints CollisionMethod(CCollider* _pOther);
};