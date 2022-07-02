#pragma once

class CGameObjectComponent
{
	
private:

protected:
	bool m_bStartCalled;

	virtual void Start() = 0;
	virtual void BeginUpdate() = 0;
	virtual void Update() = 0;
	virtual void EndUpdate() = 0;

public:


};

