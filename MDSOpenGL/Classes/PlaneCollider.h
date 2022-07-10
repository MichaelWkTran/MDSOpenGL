#pragma once
#include "Collider.h"

class CPlaneCollider : public CCollider
{
public:
	CPlaneCollider()
	{
	}
	const stCollisionPoints CollisionMethod(CCollider* _pOther);
};