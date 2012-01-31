/****************************************************************************
Copyright (c) 2010 ForzeField Studios S.L. http://forzefield.com
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

#include "CCArray2.h"

namespace cocos2d
{

SharedPtr<CCArray2> CCArray2::array()
{
    SharedPtr<CCArray2> pArray(new CCArray2());
    pArray->init();
    return pArray;
}

SharedPtr<CCArray2> CCArray2::arrayWithCapacity(unsigned int capacity)
{
    SharedPtr<CCArray2> pArray(new CCArray2());
    pArray->initWithCapacity(capacity);
    return pArray;
}

SharedPtr<CCArray2> CCArray2::arrayWithArray(SharedPtr<CCArray2> otherArray)
{
    SharedPtr<CCArray2> pArray(new CCArray2());
    pArray->initWithArray(otherArray);
    return pArray;
}

bool CCArray2::init()
{
    return initWithCapacity(1);
}

bool CCArray2::initWithCapacity(unsigned int capacity)
{
    data = ccArrayNew(capacity);
    return true;
}

bool CCArray2::initWithArray(SharedPtr<CCArray2> otherArray)
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! initWithCapacity(otherArray->data->num));

        addObjectsFromArray(otherArray);
        bRet = true;
    } while (0);
    
    return bRet;
}

unsigned int CCArray2::count()
{
    return data->num;
}

unsigned int CCArray2::capacity()
{
    return data->max;
}

unsigned int CCArray2::indexOfObject(SharedPtr<CCObject> object)
{
    return ccArrayGetIndexOfObject(data, object.get());
}

SharedPtr<CCObject> CCArray2::objectAtIndex(unsigned int index)
{
    CCAssert(index < data->num, "index out of range in objectAtIndex()");

    return SharedPtr<CCObject>(data->arr[index]);
}

SharedPtr<CCObject> CCArray2::lastObject()
{
    if( data->num > 0 )
        return SharedPtr<CCObject>(data->arr[data->num-1]);

    return SharedPtr<CCObject>(new CCObject);
}

SharedPtr<CCObject> CCArray2::randomObject()
{
    if(data->num==0) return SharedPtr<CCObject>(new CCObject);

    return SharedPtr<CCObject>(data->arr[(int)(data->num*CCRANDOM_0_1())]);
}

bool CCArray2::containsObject(SharedPtr<CCObject> object)
{
    return ccArrayContainsObject(data, object.get());
}

void CCArray2::addObject(SharedPtr<CCObject> object)
{
    ccArrayAppendObjectWithResize(data, object.get());
}

void CCArray2::addObjectsFromArray(SharedPtr<CCArray2> otherArray)
{
    ccArrayAppendArrayWithResize(data, otherArray->data);
}

void CCArray2::insertObject(SharedPtr<CCObject> object, unsigned int index)
{
    ccArrayInsertObjectAtIndex(data, object.get(), index);
}

void CCArray2::removeLastObject()
{
    CCAssert(data->num, "no objects added");
    ccArrayRemoveObjectAtIndex(data, data->num-1);
}

void CCArray2::removeObject(SharedPtr<CCObject> object)
{
    ccArrayRemoveObject(data, object.get());
}

void CCArray2::removeObjectAtIndex(unsigned int index)
{
    ccArrayRemoveObjectAtIndex(data, index);
}

void CCArray2::removeObjectsInArray(SharedPtr<CCArray2> otherArray)
{
    ccArrayRemoveArray(data, otherArray->data);
}

void CCArray2::removeAllObjects()
{
    ccArrayRemoveAllObjects(data);
}

void CCArray2::fastRemoveObjectAtIndex(unsigned int index)
{
    ccArrayFastRemoveObjectAtIndex(data, index);
}

void CCArray2::fastRemoveObject(SharedPtr<CCObject> object)
{
    ccArrayFastRemoveObject(data, object.get());
}

void CCArray2::exchangeObject(SharedPtr<CCObject> object1, SharedPtr<CCObject> object2)
{
    unsigned int index1 = ccArrayGetIndexOfObject(data, object1.get());
    if(index1 == UINT_MAX)
    {
        return;
    }

    unsigned int index2 = ccArrayGetIndexOfObject(data, object2.get());
    if(index2 == UINT_MAX)
    {
        return;
    }

    ccArraySwapObjectsAtIndexes(data, index1, index2);
}

void CCArray2::exchangeObjectAtIndex(unsigned int index1, unsigned int index2)
{
    ccArraySwapObjectsAtIndexes(data, index1, index2);
}

void CCArray2::reverseObjects()
{
    if (data->num > 1)
    {
        //floor it since in case of a oneven number the number of swaps stays the same
        int count = (int) floorf(data->num/2.f); 
        unsigned int maxIndex = data->num - 1;

        for (int i = 0; i < count ; i++)
        {
            ccArraySwapObjectsAtIndexes(data, i, maxIndex);
            maxIndex--;
        }
    }
}

void CCArray2::reduceMemoryFootprint()
{
    ccArrayShrink(data);
}

CCArray2::~CCArray2()
{
    ccArrayFree(data);
}

}
