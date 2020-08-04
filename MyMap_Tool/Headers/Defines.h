#ifndef Define_h__
#define Define_h__

extern	HWND		g_hWnd;
extern	HINSTANCE	g_hInst;

enum RESOURCEID { RESOURCE_STATIC, RESOURCE_LOGO, RESOURCE_STAGE, RESOURCE_END };

#define WINCX 1024	
#define WINCY 768

const WORD VTXCNTX = 129;
const WORD VTXCNTZ = 129;
const WORD VTXITV = 1;

const D3DXVECTOR3  g_vLook{ 0.f, 0.f, 1.f };
const D3DXVECTOR3  g_vUp{ 0.f, 1.f, 0.f };

const WORD		MIN_STR = 64;
const WORD		MIDDLE_STR = 128;
const WORD		MAX_STR = 256;

#define			MSG_BOX(_message)	MessageBox(NULL, _message, L"System Message", MB_OK)

#define NO_COPY(CLASSNAME)							\
	private:										\
	CLASSNAME(const CLASSNAME&);					\
	CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)				\
	NO_COPY(CLASSNAME)								\
	private:										\
	static CLASSNAME*	m_pInstance;				\
	public:											\
	static CLASSNAME*	GetInstance( void );		\
	static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
	if(NULL == m_pInstance) {						\
	m_pInstance = new CLASSNAME;					\
	}												\
	return m_pInstance;								\
	}												\
	void CLASSNAME::DestroyInstance( void ) {		\
	if(NULL != m_pInstance)	{						\
	delete m_pInstance;								\
	m_pInstance = NULL;								\
	}												\
	}

#define ERR_MSG(Message)		MessageBox(g_hWnd, Message, L"System Error", NULL)

#endif // !Define_h__
