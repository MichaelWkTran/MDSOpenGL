#pragma once
#include <glm/glm.hpp>

class CTransform;
class CCollider;

enum class ForceMode
{
	AddForce,
	Acceleration,
	Impulse,
	VelocityChange
};

//https://www.youtube.com/watch?v=lS_qeBy3aQI
class CRigidBody
{
protected:
	glm::vec3 m_v3PositionOld;
	CCollider* m_pCollider;

public:
	bool m_bIsKinematic;
	float m_fMass;
	float m_fGravityScale;
	float m_fLinearDrag;
	float m_fAngularDrag;
	glm::vec3 m_v3Velocity;
	glm::vec3 m_v3Acceleration;

	CRigidBody();

	void ConstructComponent(CCollider& _Collider)
	{
		m_v3PositionOld = _Collider.GetTransform()->GetPosition();
		m_pCollider = &_Collider;
	}
	void ApplyForce(glm::vec3 _v3fForce, ForceMode _Mode = ForceMode::AddForce)
	{
		switch (_Mode)
		{
		case ForceMode::AddForce:
			m_v3Acceleration += _v3fForce / m_fMass;
			break;
		case ForceMode::Acceleration:
			m_v3Acceleration += _v3fForce;
			break;
		case ForceMode::Impulse:
			m_v3Acceleration = _v3fForce / m_fMass;
			break;
		case ForceMode::VelocityChange:
			m_v3Velocity = _v3fForce;
			break;
		}
	}
	void Update();
};