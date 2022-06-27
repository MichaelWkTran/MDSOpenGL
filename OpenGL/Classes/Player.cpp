#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../GenerateMesh.h"
#include "../ExternVariables.h"

CPlayer::CPlayer()
{
    gm::GenerateSphere(m_Mesh, 0.1f, 20);

    m_Mesh.m_vTextures.push_back(CTextureManager::Insert("Resources/Textures/Planks.png", "Diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE));
    m_Mesh.m_vTextures.push_back(CTextureManager::Insert("Resources/Textures/PlanksSpecular.png", "Specular", 1, GL_RED, GL_UNSIGNED_BYTE));
    
    m_bUpdateTransform = true;
    m_mat4Model = glm::mat4x4(1);
    m_v3Position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_fquatRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    m_v3Scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

const CShader* CPlayer::GetShader() const { return m_Mesh.m_pShader; }   /**/ void CPlayer::SetShader(CShader* _Shader) { m_Mesh.m_pShader = _Shader; };
const glm::vec3& CPlayer::GetPosition() const { return m_v3Position; }    /**/ void CPlayer::SetPosition(const glm::vec3 _v3Position) { m_v3Position = _v3Position; m_bUpdateTransform = true; }
const glm::quat& CPlayer::GetRotation() const { return m_fquatRotation; } /**/ void CPlayer::SetRotation(const glm::quat _fquatRotation) { m_fquatRotation = _fquatRotation; m_bUpdateTransform = true; }
const glm::vec3& CPlayer::GetScale() const { return m_v3Scale; }          /**/ void CPlayer::SetScale(const glm::vec3 _v3Scale) { m_v3Scale = _v3Scale; m_bUpdateTransform = true; }

void CPlayer::Input(GLFWwindow* _pWindow)
{
    glm::vec3 v3MovementInput
    (
        (glfwGetKey(_pWindow, GLFW_KEY_A) == GLFW_PRESS) - (glfwGetKey(_pWindow, GLFW_KEY_D) == GLFW_PRESS),
        (glfwGetKey(_pWindow, GLFW_KEY_Q) == GLFW_PRESS) - (glfwGetKey(_pWindow, GLFW_KEY_E) == GLFW_PRESS),
        (glfwGetKey(_pWindow, GLFW_KEY_W) == GLFW_PRESS) - (glfwGetKey(_pWindow, GLFW_KEY_S) == GLFW_PRESS)
    );
    if (v3MovementInput == glm::vec3()) return void();

    SetPosition(GetPosition() + (v3MovementInput * e_fDeltatime));
}

void CPlayer::Draw(CCamera& _Camera)
{
    if (m_Mesh.m_pShader == nullptr) return void();

    if (m_bUpdateTransform)
    {
        m_mat4Model = glm::mat4x4(1);
        m_mat4Model = glm::scale(m_mat4Model, m_v3Scale) * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), m_fquatRotation * glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_mat4Model = glm::translate(m_mat4Model, m_v3Position);
        m_bUpdateTransform = false;
    }
    
    m_Mesh.m_pShader->Activate();
    glUniformMatrix4fv(glGetUniformLocation(m_Mesh.m_pShader->GetID(), "uni_mat4Model"), 1, GL_FALSE, glm::value_ptr(m_mat4Model));
    m_Mesh.Draw(_Camera);
}