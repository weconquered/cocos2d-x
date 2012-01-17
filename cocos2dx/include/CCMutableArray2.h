/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __COCOA_CC_MUTABLE_ARRAY2_H__
#define __COCOA_CC_MUTABLE_ARRAY2_H__

#include "CCObject.h"
#include "ccMacros.h"
#include <vector>
#include <stdarg.h>

namespace   cocos2d {

// the element should be pointer of CCObject or it's sub class
template<class T = SharedPtr<CCObject>>
class CCMutableArray2
{
public:
	typedef std::vector<T>								CCObjectArray;
	typedef typename CCObjectArray::iterator			CCMutableArray2Iterator;
	typedef typename CCObjectArray::reverse_iterator	CCMutableArray2RevIterator;

public:
	CCMutableArray2(unsigned int uSize = 0)
	{
		if (uSize != 0)
			m_array.reserve(uSize);
	}

	virtual ~CCMutableArray2(void)
	{
		removeAllObjects();
	}

    static SharedPtr<CCMutableArray2<T>> newMutableArray()
    {
        return SharedPtr<CCMutableArray2<T>>(new CCMutableArray2<T>());
    }
	
    inline unsigned int count(void)
	{
		return (unsigned int)m_array.size();
	}

	unsigned int getIndexOfObject(T pObject)
	{
                if (m_array.empty() || (pObject == 0))
		{
			return 0;
		}

		CCMutableArray2Iterator iter;
		unsigned int uRet = 0;
		unsigned int i;
		for (iter = m_array.begin(), i = 0; iter != m_array.end(); ++iter, ++i)
		{
			if (*iter == pObject)
			{
				uRet = i;
				break;
			}
		}

		return uRet;
	}

	bool containsObject(T pObject)
	{
		if (m_array.empty() || (! pObject))
		{
			return false;
		}

		bool bRet = false;
		CCMutableArray2Iterator iter;
		for (iter = m_array.begin(); iter != m_array.end(); ++iter)
		{
			if (*iter == pObject)
			{
				bRet = true;
				break;
			}
		}

		return bRet;
	}

	T getLastObject(void)
	{
		CCMutableArray2RevIterator iter = rbegin();

		if (iter != m_array.rend())
			return *iter;

                return 0;
	}

	T getObjectAtIndex(unsigned int uIndex)
	{
		CCAssert(uIndex < count(), "");

		if (uIndex >= count())
		{
                        return 0;
		}

		return m_array[uIndex];
	}

	// Adding objects
	void addObject(T pObject)
	{
		// make sure the pointer is not null
                if (pObject == 0)
		{
			return;
		}

		m_array.push_back(pObject);
	}

	void addObjectsFromArray(CCMutableArray2<T> *pArray)
	{
		if (pArray && pArray->count() > 0)
		{
			m_array.reserve(count() + pArray->count());
			CCMutableArray2Iterator iter;
			for (iter = pArray->begin(); iter != pArray->end(); ++iter)
			{
				m_array.push_back(*iter);
			}
		}
	}

    void insertObjectAtIndex(T pObject, unsigned int uIndex)
	{
		CCAssert(uIndex <= count(), "");
		// make sure the object is not null
                if (pObject == 0)
		{
			return;
		}

		// resize the capacity if the index out of it
		if (uIndex >= m_array.capacity())
		{
			m_array.reserve(uIndex + 1);
			m_array.push_back(pObject);
		}
		else	// insert the object
			m_array.insert(m_array.begin() + uIndex, pObject);
	}

	// Removing objects
	void removeLastObject(bool bDeleteObject = true)
	{
		CCMutableArray2RevIterator it = m_array.rbegin();
		if (it != m_array.rend())
		{
			m_array.pop_back();
		}
	}

	void removeObject(T pObject, bool bDeleteObject = true)
	{
		if (m_array.empty() || (! pObject))
		{
			return;
		}

		CCMutableArray2Iterator iter;
		int i;
		for (iter = m_array.begin(), i = 0; iter != m_array.end(); ++iter, ++i)
		{
			if (*iter == pObject)
			{
				m_array.erase(iter);
                
				break;
			}
		}
	}

	void removeObjectsInArray(CCMutableArray2<T>* pDeleteArray)
	{
		if(pDeleteArray && pDeleteArray->count())
		{
			CCMutableArray2Iterator it;
			for( it = pDeleteArray->m_array.begin(); it != pDeleteArray->m_array.end(); ++it)
			{
				removeObject(*it);
			}
		}
	}

	void removeObjectAtIndex(unsigned int uIndex, bool bDeleteObject = true)
	{
		if (m_array.empty())
		{
			return;
		}
		
		m_array.erase(m_array.begin() + uIndex);
	}

	void removeAllObjects(bool bDeleteObject = true)
	{
		m_array.clear();
	}

	void replaceObjectAtIndex(unsigned int uIndex, T pObject, bool bDeleteObject = true)
    {
		m_array[uIndex] = pObject;
	}

	inline CCMutableArray2Iterator begin(void)
	{
		return m_array.begin();
	}

	inline CCMutableArray2RevIterator rbegin(void)
	{
		return m_array.rbegin();
	}

	CCMutableArray2Iterator getLastValidIterator(void)
	{
		CCMutableArray2Iterator iter;
		CCMutableArray2Iterator ret;
		for (iter = m_array.begin(); iter != m_array.end(); ++iter)
		{
			ret = iter;
			if (! (*iter))
			{
				break;
			}
		}

		return ret;
	}

	/*
	 * end is a keyword of lua, so should use other name
	 * to export to lua
	*/
	inline CCMutableArray2Iterator endToLua(void)
	{
		return m_array.end();
	}

	inline CCMutableArray2Iterator end(void)
	{
		return m_array.end();
	}

	inline CCMutableArray2RevIterator rend(void)
	{
		return m_array.rend();
	}

	CCMutableArray2<T>* copy(void)
	{
		return NULL;
	}

public:
	static CCMutableArray2<T>* arrayWithObjects(T pObject1, ...)
	{
		return NULL;
	}

	static CCMutableArray2<T>* arrayWithArray(CCMutableArray2<T> *pSrcArray)
	{
        return NULL;
	}

private:
	std::vector<T> m_array;
};

}//namespace   cocos2d 

#endif // __COCOA_CC_MUTABLE_ARRAY_H__
