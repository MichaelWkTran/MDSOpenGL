#include "RigidBody.h"
#include "Transform.h"
#include "Collider.h"
#include "../ExternVariables.h"
#include <iostream>

CRigidBody::CRigidBody()
{
	m_bIsKinematic = false;
	m_fMass = 1.0f;
	m_fGravityScale = 1.0f;
	m_fLinearDrag = 0.0f;
	m_fAngularDrag = 0.0f;
	m_v3PositionOld = glm::vec3();
	m_v3Velocity = glm::vec3();
	m_v3Acceleration = glm::vec3();
}

void CRigidBody::Update()
{
	//Ensure that a m_pCollider has been given
	if (m_pCollider == nullptr)
	{
		std::cout << "ERROR: Rigidbody must have m_pCollider assigned. Make sure you call ConstructComponent";
		return;
	}

	//Ensure no forces affect the rigidbody
	if (m_bIsKinematic) return;

	//Get Transform
	CTransform* pTransform = m_pCollider->GetTransform();

	//Add Gravity Force
	ApplyForce(glm::vec3(0, -1, 0) * e_fGravity * m_fGravityScale * m_fMass);

	//Apply Linear Drag
	m_v3Acceleration -= glm::normalize(m_v3Velocity) * m_fLinearDrag;

	//Add Collision Force


	//Apply Force to Velocity
	m_v3Velocity += m_v3Acceleration * e_fDeltatime;

	//Apply Physics
	pTransform->SetPosition(pTransform->GetPosition() + (m_v3Velocity * e_fDeltatime));
	
	//Reset Force
	m_v3Acceleration = glm::vec3();

	//Set New Velocity
	m_v3Velocity = pTransform->GetPosition() - m_v3PositionOld;

	//Save Current Position
	m_v3PositionOld = pTransform->GetPosition();
}