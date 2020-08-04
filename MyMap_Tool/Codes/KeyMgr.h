#pragma once
#include "Include.h"

// 싱글톤 패턴: 전역으로 필요한 객체를 하나만 할당하여 전역으로 사용하게끔 해주는 디자인 패턴
// 은닉화를 파괴하는 디자인 패턴이지만 게임 로직에서는 필수이다.
#define MAX_VIRTUAL_KEY 0xFF

class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)

private: // 싱글톤 객체의 생성자와 소멸자는 private로 제한을 둔다.
	CKeyMgr();	
	~CKeyMgr();

public:
	bool GetKeyPressing(int iKey);
	bool GetKeyDown(int iKey);
	bool GetKeyUp(int iKey);	

public:
	// Update함수는 GetKeyPressing, GetKeyDown, GetKeyUp 등을 호출한 후에 호출해주는 함수.
	void Update();	

private:
	bool m_bKey[MAX_VIRTUAL_KEY];
};

#define KEY_DOWN(Key) CKeyMgr::GetInstance()->GetKeyDown(Key)
#define KEY_UP(Key) CKeyMgr::GetInstance()->GetKeyUp(Key)
#define KEY_PRESSING(Key) CKeyMgr::GetInstance()->GetKeyPressing(Key)
