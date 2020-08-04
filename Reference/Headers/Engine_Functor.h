#ifndef Engine_Functor_h__
#define Engine_Functor_h__

#include "Engine_Typedef.h"


namespace Engine
{
	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const _tchar* pTag)
			: m_pTargetTag(pTag)		{		}
		~CTag_Finder()		{		}

	public:
		template<typename T>
		bool operator()(const T& pair)
		{
			if (0 == lstrcmpW(m_pTargetTag, pair.first))
			{
				return true;
			}

			return false;
		}

	private:
		const _tchar*		m_pTargetTag = nullptr;
	};

	class CSTD_Tag_Finder
	{
	public:
		explicit CSTD_Tag_Finder(const wstring pTag)
			: m_pTargetTag(pTag) {		}
		~CSTD_Tag_Finder() {		}
	public:
		template<typename T>
		bool operator()(const T& Stdpair)
		{
			if (m_pTargetTag == Stdpair.first)
			{
				return true;
			}

			return false;
		}

	private:
		const wstring		m_pTargetTag = nullptr;
	};

	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public: // operator
		template <typename T>
		void operator () (T& pInstance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
	};

	//// 연관컨테이너 삭제용
	//class CDeleteMap
	//{
	//public:
	//	explicit CDeleteMap(void) {}
	//	~CDeleteMap(void) {}
	//public: // operator	
	//	template <typename T> 
	//	void operator () (T& Pair)
	//	{
	//		if(NULL != Pair.second)
	//		{
	//			delete Pair.second;
	//			Pair.second = NULL;
	//		}
	//	}
	//};
	
	// 연관컨테이너 삭제용
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = NULL;
		}
	};
}


#endif // Engine_Functor_h__
