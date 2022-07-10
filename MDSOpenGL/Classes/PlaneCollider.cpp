#include "PlaneCollider.h"
#include "Transform.h"

using namespace CollisionMethodAlgorithms;

const stCollisionPoints CPlaneCollider::CollisionMethod(CCollider* _pOther)
{
	return stCollisionPoints();
}