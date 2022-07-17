#include "RigidBody.h"
#include "Transform.h"
#include "../ExternVariables.h"

CRigidBody::CRigidBody()
{
	m_pTransform = nullptr;
	m_bIsKinematic = false;
	m_fMass = 1.0f;
	m_fGravityScale = 1.0f;
	m_fLinearDrag = 0.0f;
	m_AngularDrag = 0.0f;
	m_v3PositionOld = glm::vec3();
	m_v3Velocity = glm::vec3();
	m_v3Acceleration = glm::vec3();
}

void CRigidBody::ConstructComponent(CTransform& _Transform)
{
	m_pTransform = &_Transform;
	m_v3PositionOld = _Transform.GetPosition();
}

void CRigidBody::Update()
{
	//Ensure that a m_pTransform has been given
	if (m_pTransform == nullptr) return;

	//Add Gravity Force
	m_v3Acceleration += glm::vec3(0, -1, 0) * e_fGravity * m_fGravityScale;

	//Apply Linear Drag
	m_v3Acceleration -= m_fLinearDrag;

	//Apply Force to Velocity
	m_v3Velocity += m_v3Acceleration * e_fDeltatime;

	//Apply Physics
	m_pTransform->SetPosition(m_pTransform->GetPosition() + (m_v3Velocity * e_fDeltatime));
	
	//Reset Force
	m_v3Acceleration = glm::vec3();

	//Set New Velocity
	m_v3Velocity = m_pTransform->GetPosition() - m_v3PositionOld;

	//Save Current Position
	m_v3PositionOld = m_pTransform->GetPosition();
}