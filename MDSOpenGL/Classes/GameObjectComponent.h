#pragma once
class CGameObject;
class CCamera;

class CGameObjectComponent
{
private:
	bool m_bStartCalled;

public:
	CGameObject* m_pGameObject;
	
	CGameObjectComponent()
	{
		m_bStartCalled = false;
		m_pGameObject = nullptr;
	}

	virtual void Start()
	{
		if (m_bStartCalled) return;
		m_bStartCalled = true;
	};
	virtual void BeginUpdate() {};
	virtual void Update() {};
	virtual void EndUpdate() {};
	virtual void Draw(const CCamera&) {};
};