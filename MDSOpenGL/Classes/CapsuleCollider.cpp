#include "CapsuleCollider.h"
#include "Transform.h"

using namespace CollisionMethodAlgorithms;

const stCollisionPoints CCapsuleCollider::CollisionMethod(CCollider* _pOther)
{
    return stCollisionPoints();
}