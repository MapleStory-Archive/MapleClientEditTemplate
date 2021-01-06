#pragma once

struct ZListDataVtbl
{
    void* (__thiscall* __vecDelDtor)(unsigned int);
};

template <typename T>
struct ZListData
{
    ZListDataVtbl* vfptr;
    ZListData<T>* pNext;
    ZListData<T>* pPrev;
    char aPad[4];
    T data;
};

template <typename T>
class ZList
{
private:
    void* vfptr;
    void* padding;

public:
    unsigned int m_uCount;
    T* m_pHead;
    T* m_pTail;

    ZList()
    {
        vfptr = 0;
        padding = 0;

        m_uCount = 0;
        m_pHead = 0;
        m_pTail = 0;
    }

    T* GetNext(T** ppPosition)
    {
        T* pResult = *ppPosition;

        ZListData<T>* pData = reinterpret_cast<ZListData<T>*>(reinterpret_cast<unsigned char*>(pResult) - 16);

        if (pData->pNext)
            *ppPosition = &pData->pNext->data;
        else
            *ppPosition = nullptr;

        return pResult;
    }

    void RemoveAll()
    {
        auto pPosition = this->m_pHead;

        while (pPosition)
        {
            T* pItem = this->GetNext(&pPosition);

            ZListData<T>* pData = reinterpret_cast<ZListData<T>*>(reinterpret_cast<unsigned char*>(pItem) - 16);

            pData->vfptr->__vecDelDtor(1);

        }

        this->m_pTail = 0;
        this->m_pHead = 0;
        this->m_uCount = 0;
    }
};