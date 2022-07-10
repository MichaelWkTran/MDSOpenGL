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
	m_v3Force = glm::vec3();
}

void CRigidBody::SetTransform(CTransform& _Transform)
{
	m_pTransform = &_Transform;
	m_v3PositionOld = _Transform.GetPosition();
}

void CRigidBody::BeginUpdate()
{
	//Calculate Velocity (In BeginUpdate so that it can be overridden before being used)
	m_v3Velocity = m_pTransform->GetPosition() - m_v3PositionOld;
}

void CRigidBody::Update()
{
	//Save Current Position
	m_v3PositionOld = m_pTransform->GetPosition();
	
	//Add Gravity Force
	m_v3Force += glm::vec3(0,-1,0) * e_fGravity * m_fGravityScale;
	
	//Apply Linear Drag
	m_v3Force -= m_fLinearDrag;

	//Apply Physics
	m_pTransform->SetPosition(m_pTransform->GetPosition() + m_v3Velocity + ((m_v3Force / m_fMass) * e_fDeltatime * e_fDeltatime));
	
	//Reset Force
	m_v3Force = glm::vec3();
}