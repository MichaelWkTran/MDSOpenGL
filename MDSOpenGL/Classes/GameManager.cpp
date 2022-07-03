#include "GameManager.h"

CGameManager::CGameManager()
{
	m_pCamera = nullptr;
}

CGameManager::~CGameManager()
{
	Clear();
}

void CGameManager::Clear()
{
	const int iGameObjectsCount = m_dequeGameObject.size();
	for (int i = 0; i < iGameObjectsCount; i++)
	{
		delete m_dequeGameObject.front();
		m_dequeGameObject.pop_front();
	}
}

void CGameManager::Update()
{
	for (auto& pGameObject : m_dequeGameObject) pGameObject->Start();
	for (auto& pGameObject : m_dequeGameObject) pGameObject->BeginUpdate();
	for (auto& pGameObject : m_dequeGameObject) pGameObject->Update();
	for (auto& pGameObject : m_dequeGameObject) pGameObject->EndUpdate();
	for (auto& pGameObject : m_dequeGameObject) pGameObject->Draw(*m_pCamera);

	//Delete GameObjects
	for (int i = 0; i < (int)m_dequeGameObject.size(); i++)
	{
		if (!m_dequeGameObject[i]->GetDeleteGameObject()) continue;

		CGameObject* pDeletedGameObject = m_dequeGameObject[i];
		m_dequeGameObject.erase(m_dequeGameObject.begin() + i);
		delete pDeletedGameObject;
	}
}