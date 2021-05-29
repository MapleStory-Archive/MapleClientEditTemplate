#pragma once
#include "ZRefCountedAccessor.h"
#include "ZRefCountedDummy.h"

/// <summary>
/// Emulation of MapleStory's implementation of a doubly linked list template.
/// 
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class ZList : ZRefCountedAccessor<T>, ZRefCountedAccessor<ZRefCountedDummy<T>>
{
private:
	char gap4[1];
	size_t m_uCount;
	T* m_pHead;
	T* m_pTail;
public:

	/***=========== CONSTRUCTORS ===========***/

	ZList()
	{
		m_uCount = 0;
		m_pHead = nullptr;
		m_pTail = nullptr;
	}

	ZList(ZList<T>* l)
	{
		m_uCount = 0;
		m_pHead = nullptr;
		m_pTail = nullptr;

		this->RemoveAll();
		this->AddTail(l);
	}

	virtual ~ZList()
	{
		this->RemoveAll();
	}

	/***=========== OPERATOR OVERLOAD ===========***/

	ZList<T>* operator=(ZList<T>* l)
	{
		this->RemoveAll();
		this->AddTail(l);

		return this;
	}

	/***=========== MEMBER VARIABLE ACCESSORS ===========***/

	T* GetHeadPosition()
	{
		return this->m_pHead;
	}

	T* GetTailPosition()
	{
		return this->m_pTail;
	}

	size_t GetCount()
	{
		return this->m_uCount;
	}

	/***=========== ADD HEAD ===========***/

	T* AddHead()
	{
		T* pAlloc = this->New(this->m_pHead, nullptr);

		if (!this->m_pTail)
		{
			this->m_pTail = pAlloc;
		}
		else
		{
			ZRefCountedDummy<T>* pNode = pAlloc ? this->CastNode(pAlloc) : nullptr;
			ZRefCountedDummy<T>* pHeadNode = this->CastNode(this->m_pHead);

			pHeadNode->m_pNext = pNode;
		}

		this->m_pHead = pAlloc;

		return pAlloc;
	}

	T* AddHead(T* d)
	{
		T* pNewHead = this->AddHead();
		*pNewHead = *d;
		return pNewHead;
	}

	T* AddHead(ZList<T>* l)
	{
		return nullptr; // TODO
	}

	/***=========== ADD TAIL ===========***/

	T* AddTail()
	{
		T* pAlloc = this->New(this->m_pTail, nullptr);

		if (this->m_pTail)
		{
			ZRefCountedDummy<T>* pNode = pAlloc ? this->CastNode(pAlloc) : nullptr;
			ZRefCountedDummy<T>* pTailNode = this->CastNode(this->m_pTail);

			pTailNode->m_pNext = pNode;
			this->m_pTail = pAlloc;
		}
		else
		{
			this->m_pHead = pAlloc;
			this->m_pTail = pAlloc;
		}

		return pAlloc;
	}

	T* AddTail(T* d)
	{
		T* pNewTail = this->AddTail();
		*pNewTail = *d;
		return pNewTail;
	}

	T* AddTail(ZList<T>* l)
	{
		T* pHead = l->m_pHead;

		while (pHead)
		{
			T* pNext = pHead;

			ZRefCountedDummy<T>* pNode = this->CastNode(pHead);
			ZRefCountedDummy<T>* pNodePrev = this->CastNode(pNode->m_pPrev);

			pHead = pNodePrev ? pNodePrev->t : nullptr;

			T* pNew = this->AddTail();
			*pNew = *pNext;
		}
	}

	/***=========== NODE REMOVAL ===========***/

	void RemoveAll()
	{
		T* pPosition = this->GetHeadPosition();

		while (pPosition)
		{
			T* pItem = this->GetNext(&pPosition);

			pItem->~T();
		}

		this->m_pTail = nullptr;
		this->m_pHead = nullptr;
		this->m_uCount = 0;
	}

	void RemoveAt(T* pos)
	{
		// TODO
	}

	/***=========== NODE SEARCH ===========***/

	T* FindIndex(const size_t uIndex)
	{
		// TODO
	}

	int IndexOf(T* pos)
	{
		// TODO
	}

	/***=========== INSERTION ===========***/

	T* Insert(T* d)
	{
		T* result = this->AddTail(this);
	}

	T** InsertBefore(T* pos)
	{

	}

	/***=========== TRAVERSAL ===========***/

	T* GetNext(T** pos)
	{
		T* pRet = *pos;

		ZRefCountedDummy<T>* pNode = this->CastNode(pRet);

		*pos = pNode->m_pNext ? &this->CastNode((T*)pNode->m_pNext)->t : nullptr;

		return pRet;
	}

	T* GetPrev(T** pos)
	{
		T* pRet = *pos;

		ZRefCountedDummy<T>* pNode = this->CastNode(pRet);

		*pos = pNode->m_pPrev ? &this->CastNode((T*)pNode->m_pPrev)->t : nullptr;

		return pRet;
	}

private:

	/// <summary>
	/// If t is not a ZList member, then this will produce undefined results
	/// </summary>
	/// <param name="t"></param>
	/// <returns></returns>
	ZRefCountedDummy<T>* CastNode(T* t)
	{
		return reinterpret_cast<ZRefCountedDummy<T>*>(reinterpret_cast<char*>(t) - 16);
	}

	T* New(T* pPrev, T* pNext)
	{
		ZRefCountedDummy<T>* pAlloc = new ZRefCountedDummy<T>(); // IDA: ZRefCounted_Alloc<ZRefCountedDummy<T>>();

		pAlloc->m_pPrev = pPrev ? this->CastNode(pPrev) : nullptr;
		pAlloc->m_pNext = pNext ? this->CastNode(pNext) : nullptr;
		pAlloc->m_nRef = 0;

		this->m_uCount += 1;

		return &pAlloc->t;
	}
};

assert_size(sizeof(ZList<int>), 0x14);