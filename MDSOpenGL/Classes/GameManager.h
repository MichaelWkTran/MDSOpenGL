#pragma once
#include "GameObject.h"
#include "Camera.h"
#include <deque>

class CGameManager
{
private:
	std::deque<CGameObject*> m_dequeGameObject;
	
public:
	CCamera* m_pCamera;
	
	CGameManager();
	~CGameManager();

	template <class T>
	T* CreateObject();
	void Clear();

	void Update();
};

template<class T>
inline T* CGameManager::CreateObject()
{
	m_dequeGameObject.emplace_back(new T);
	m_dequeGameObject.back()->m_pGameManager = this;
	return (T*)m_dequeGameObject.back();
}