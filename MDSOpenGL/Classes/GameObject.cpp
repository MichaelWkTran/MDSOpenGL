#include "GameManager.h"
#include "GameObject.h"
#include "GameObjectComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CGameObject::CGameObject()
{
    m_pGameManager = nullptr;
    
    m_bUpdateTransform = true;
    m_v3Position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_fquatRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    m_v3Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    m_mat4Model = glm::mat4x4(1);

    m_bDeleteGameObject = false;
}

#pragma region  Get Set Methods
const glm::mat4x4 CGameObject::GetModel() const
{
    return m_mat4Model;
}

const glm::vec3 CGameObject::GetPosition() const
{
    return m_v3Position;
}
void CGameObject::SetPosition(const glm::vec3 _v3Position)
{
    m_v3Position = _v3Position;
    m_bUpdateTransform = true;
}

const glm::quat CGameObject::GetRotation() const
{
    return m_fquatRotation;
}
void CGameObject::SetRotation(const glm::fquat _fquatRotation)
{
    m_fquatRotation = _fquatRotation;
    m_bUpdateTransform = true;
}

const glm::vec3 CGameObject::GetRotationEuler(bool _InDegrees/* = true*/) const
{
    glm::vec3 v3Output = glm::eulerAngles(m_fquatRotation);
    if (_InDegrees) v3Output = glm::degrees(v3Output);

    return v3Output;
}
void CGameObject::SetRotationEuler(const glm::vec3 _v3Rotation, bool _InDegrees/* = true*/)
{
    m_fquatRotation = _InDegrees ? glm::quat(glm::radians(_v3Rotation)) : glm::quat(_v3Rotation);
}

const glm::vec3 CGameObject::GetScale() const
{
    return m_v3Scale;
}
void CGameObject::SetScale(const glm::vec3 _v3Scale)
{
    m_v3Scale = _v3Scale;
    m_bUpdateTransform = true;
}

const bool CGameObject::GetDeleteGameObject() const
{ 
    return m_bDeleteGameObject;
}
void CGameObject::DeleteGameObject()
{
    m_bDeleteGameObject = true;
}

const glm::vec3 CGameObject::Up() const
{
    return m_fquatRotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

const glm::vec3 CGameObject::Forward() const
{
    return m_fquatRotation * glm::vec3(0.0f, 0.0f, 1.0f);
}

const glm::vec3 CGameObject::Right() const
{
    return m_fquatRotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

#pragma endregion

void CGameObject::Start()
{
    for (auto pComponent : m_vComponents) pComponent->Start();
}

void CGameObject::BeginUpdate()
{
    for (auto pComponent : m_vComponents) pComponent->BeginUpdate();
}

void CGameObject::Update()
{
    for (auto pComponent : m_vComponents) pComponent->Update();
}

void CGameObject::EndUpdate()
{
    for (auto pComponent : m_vComponents) pComponent->EndUpdate();
}

void CGameObject::Draw(const CCamera& _Camera)
{
    if (m_bUpdateTransform)
    {
        m_mat4Model = glm::mat4x4(1);
        m_mat4Model = glm::scale(m_mat4Model, m_v3Scale);
        m_mat4Model *= glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), Forward(), glm::vec3(0.0f, 1.0f, 0.0f));
        m_mat4Model = glm::translate(m_mat4Model, m_v3Position);
        m_bUpdateTransform = false;
    }

    for (auto pComponent : m_vComponents) pComponent->Draw(_Camera);
}
