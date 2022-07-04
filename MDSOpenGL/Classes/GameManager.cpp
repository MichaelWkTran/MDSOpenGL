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
	const int iGameObjectsCount = m_dequeUpdatedObject.size();
	for (int i = 0; i < iGameObjectsCount; i++)
	{
		delete m_dequeUpdatedObject.front();
		m_dequeUpdatedObject.pop_front();
	}
}

void CGameManager::Update()
{
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->Start();
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->BeginUpdate();
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->Update();
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->EndUpdate();
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->Draw(*m_pCamera);

	//Delete GameObjects
	for (int i = 0; i < (int)m_dequeUpdatedObject.size(); i++)
	{
		if (!m_dequeUpdatedObject[i]->GetDeleteUpdatedObject()) continue;

		CUpdatedObject* pDeletedGameObject = m_dequeUpdatedObject[i];
		m_dequeUpdatedObject.erase(m_dequeUpdatedObject.begin() + i);
		delete pDeletedGameObject;
	}
}