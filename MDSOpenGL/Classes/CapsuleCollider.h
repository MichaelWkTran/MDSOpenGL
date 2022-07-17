#pragma once
#include "Collider.h"

class CCapsuleCollider : public CCollider
{
public:
	float m_fRadius;
	float m_fHeight;

	CCapsuleCollider(float _fRadius, float _fHeight)
	{
		m_fRadius = _fRadius;
		m_fHeight = _fHeight;
	}
	~CCapsuleCollider() {}
	virtual const stCollisionPoints CollisionMethod(CCollider* _pOther) override;
};