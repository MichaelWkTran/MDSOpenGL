#include "SphereCollider.h"
#include "Transform.h"

using namespace CollisionMethodAlgorithms;

const stCollisionPoints CSphereCollider::CollisionMethod(CCollider* _pOther)
{
	if (dynamic_cast<CSphereCollider*>(_pOther)) return SphereSphereCollision(this, _pOther);
	
}
