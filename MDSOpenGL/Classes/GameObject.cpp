#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../GenerateMesh.h"
#include "../ExternVariables.h"

CGameObject::CGameObject()
{
    m_bUpdateTransform = true;
    m_mat4Model = glm::mat4x4(1);
    m_v3Position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_fquatRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    m_v3Scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

const glm::mat4x4 CGameObject::GetModel() const { return m_mat4Model; }
const glm::vec3 CGameObject::GetPosition() const { return m_v3Position; }    /**/ void CGameObject::SetPosition(const glm::vec3 _v3Position) { m_v3Position = _v3Position; m_bUpdateTransform = true; }
const glm::quat CGameObject::GetRotation() const { return m_fquatRotation; } /**/ void CGameObject::SetRotation(const glm::fquat _fquatRotation) { m_fquatRotation = _fquatRotation; m_bUpdateTransform = true; }
const glm::vec3 CGameObject::GetScale() const { return m_v3Scale; }          /**/ void CGameObject::SetScale(const glm::vec3 _v3Scale) { m_v3Scale = _v3Scale; m_bUpdateTransform = true; }

void CGameObject::Draw(const CCamera& _Camera)
{
    if (m_Mesh.m_pShader == nullptr) return void();
    
    if (m_bUpdateTransform)
    {
        m_mat4Model = glm::mat4x4(1);
        m_mat4Model = glm::scale(m_mat4Model, m_v3Scale);
        m_mat4Model *= glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), m_fquatRotation * glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f,1.0f,0.0f));
        m_mat4Model = glm::translate(m_mat4Model, m_v3Position);
        m_bUpdateTransform = false;
    }
    
    m_Mesh.m_pShader->Activate();
    glUniformMatrix4fv(glGetUniformLocation(*m_Mesh.m_pShader, "uni_mat4Model"), 1, GL_FALSE, glm::value_ptr(m_mat4Model));

    m_Mesh.Draw(_Camera);
    m_Mesh.m_pShader->Deactivate();
}