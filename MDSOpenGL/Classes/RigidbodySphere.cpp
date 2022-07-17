#include "RigidbodySphere.h"
#include "../GenerateMesh.h"
#include "Camera.h"

CRigidbodySphere::CRigidbodySphere()
{
	m_Rigidbody.ConstructComponent(m_Transform);
	m_Mesh.m_pTransform = &m_Transform;
	gm::GenerateSphere(m_Mesh, 0.5f, 20);
	m_Collider.ConstructComponent(&m_Transform, &m_Rigidbody);
}

void CRigidbodySphere::Update()
{
	m_Rigidbody.Update();
	m_Collider.CheckCollision();
}

void CRigidbodySphere::Draw(const CCamera& _Camera)
{
	m_Mesh.Draw(_Camera);
}