#pragma once
#include "RigidBody.h"
#include "UpdatedObject.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "Mesh.h"

class CCamera;

class CRigidbodySphere : public CUpdatedObject
{
protected:

public:
	CTransform m_Transform;
	CMesh<> m_Mesh;
	CRigidBody m_Rigidbody;
	CSphereCollider m_Collider;

	CRigidbodySphere();
	~CRigidbodySphere() {};

	virtual void Update() override;
	virtual void Draw(const CCamera& _Camera) override;
};