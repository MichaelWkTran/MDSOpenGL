#pragma once
#include <deque>
#include "UpdatedObject.h"
#include "Camera.h"

class CGameManager
{
private:
	std::deque<CUpdatedObject*> m_dequeUpdatedObject;
	
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
	m_dequeUpdatedObject.emplace_back(new T);
	m_dequeUpdatedObject.back()->m_pGameManager = this;
	return (T*)m_dequeUpdatedObject.back();
}