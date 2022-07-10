#pragma once
#include "UpdatedObject.h"
#include <glm/glm.hpp>

class CTransform;

//https://www.youtube.com/watch?v=lS_qeBy3aQI
class CRigidBody : public CUpdatedObject
{
private:
	glm::vec3 m_v3PositionOld;
	CTransform* m_pTransform;

public:
	bool m_bIsKinematic;
	float m_fMass;
	float m_fGravityScale;
	float m_fLinearDrag;
	float m_AngularDrag;
	glm::vec3 m_v3Velocity;
	glm::vec3 m_v3Force;

	CRigidBody();

	void SetTransform(CTransform& _Transform);
	void BeginUpdate();
	void Update();
};