//
//  PSDSmartPtr.h
//  psdtool
//
//  Created by xhnsworks on 5/11/15.
//  Copyright (c) 2015 徐 海宁. All rights reserved.
//

#ifndef __psdtool__PSDSmartPtr__
#define __psdtool__PSDSmartPtr__

class PSDRefObject
{
public:
#if defined (_WIN32) || defined (_WIN64)
    mutable volatile long ref_count;
#else
    mutable volatile long long ref_count;
#endif
    PSDRefObject()
    {
        ref_count = 0;
    }
    virtual ~PSDRefObject()
    {
    }
};

#if defined (_WIN32) || defined (_WIN64)
#include <windows.h>
inline long PSDAtomicIncrement(volatile long* i)
{
    return InterlockedIncrement((volatile LONG*)i);
}
inline long PSDAtomicDecrement(volatile long* i)
{
    return InterlockedDecrement((volatile LONG*)i);
}
#elif defined (__APPLE__)
#include <libkern/OSAtomic.h>
inline int64_t PSDAtomicIncrement(volatile int64_t* i)
{
    return OSAtomicIncrement64Barrier(i);
}
inline int64_t PSDAtomicDecrement(volatile int64_t* i)
{
    return OSAtomicDecrement64Barrier(i);
}
#endif

template< typename T >
class PSDSmartPtr
{
private:
    T* m_ptr;
public:
    inline void _inc(T* ptr)
    {
        if (ptr)
        {
            PSDAtomicIncrement(&ptr->ref_count);
        }
    }
    inline void _dec(T* ptr)
    {
        if (ptr) {
            if (!PSDAtomicDecrement(&ptr->ref_count)) {
                delete ptr;
            }
        }
    }
    PSDSmartPtr()
    : m_ptr(NULL)
    {}
    PSDSmartPtr(const PSDSmartPtr& ptr)
    {
        _inc(ptr.m_ptr);
        m_ptr = ptr.m_ptr;
    }
    PSDSmartPtr(T* ptr)
    {
        _inc(ptr);
        m_ptr = ptr;
    }
    ~PSDSmartPtr()
    {
        _dec(m_ptr);
        m_ptr = NULL;
    }
    inline T* operator = (T* ptr)
    {
        _inc(ptr);
        _dec(m_ptr);
        m_ptr = ptr;
        return m_ptr;
    }
    inline T* operator = (const PSDSmartPtr ptr)
    {
        _inc(ptr.m_ptr);
        _dec(m_ptr);
        m_ptr = ptr.m_ptr;
        return m_ptr;
    }
    inline bool operator < (const PSDSmartPtr& ptr) const
    {
        return m_ptr < ptr.m_ptr;
    }
    inline bool operator == (const PSDSmartPtr& ptr) const
    {
        return m_ptr == ptr.m_ptr;
    }
    inline bool operator != (const PSDSmartPtr& ptr) const
    {
        return m_ptr != ptr.m_ptr;
    }
    inline T* get()
    {
        return m_ptr;
    }
    inline const T* get() const
    {
        return m_ptr;
    }
    inline T* operator ->() {
        return m_ptr;
    }
    inline const T* operator ->() const {
        return m_ptr;
    }
    inline bool operator!() const {
        return !m_ptr;
    }
    inline operator bool () const {
        return m_ptr != NULL;
    }
    
};

#endif /* defined(__psdtool__PSDSmartPtr__) */
