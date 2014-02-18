/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies

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

#ifndef __CCREF_H__
#define __CCREF_H__

#include "CCDataVisitor.h"
#include "ccMacros.h"
#include "CCConsole.h"

NS_CC_BEGIN

/**
 * @addtogroup base_nodes
 * @{
 */

class Ref;
class Node;

/** Interface that defines how to clone an Ref */
class CC_DLL Clonable
{
public:
	/** returns a copy of the Ref */
    virtual Clonable* clone() const = 0;
    /**
     * @js NA
     * @lua NA
     */
	virtual ~Clonable() {};

    /** returns a copy of the Ref.
     @deprecated Use clone() instead
     */
    CC_DEPRECATED_ATTRIBUTE Ref* copy() const
    {
        // use "clone" instead
        CC_ASSERT(false);
        return nullptr;
    }
};

class CC_DLL Ref
{
public:
    /**
     * Constructor
     *
     * The Ref's reference count is 1 after construction.
     * @js NA
     */
    Ref();
    
    /**
     * @js NA
     * @lua NA
     */
    virtual ~Ref();
    
    /**
     * Release the ownership immediately.
     *
     * This decrements the Ref's reference count.
     *
     * If the reference count reaches 0 after the descrement, this Ref is
     * destructed.
     *
     * @see retain, autorelease
     * @js NA
     */
    void release();

    /**
     * Retains the ownership.
     *
     * This increases the Ref's reference count.
     *
     * @see release, autorelease
     * @js NA
     */
    inline void retain()
    {
        CCASSERT(_referenceCount > 0, "reference count should greater than 0");
        ++_referenceCount;
    }

    /**
     * Release the ownership sometime soon automatically.
     *
     * This descrements the Ref's reference count at the end of current
     * autorelease pool block.
     *
     * If the reference count reaches 0 after the descrement, this Ref is
     * destructed.
     *
     * @returns The Ref itself.
     *
     * @see AutoreleasePool, retain, release
     * @js NA
     * @lua NA
     */
    Ref* autorelease();

    /**
     * Returns the Ref's current reference count.
     *
     * @returns The Ref's reference count.
     * @js NA
     */
    unsigned int getReferenceCount() const;

protected:
    /// count of references
    unsigned int _referenceCount;
    
    friend class AutoreleasePool;
};


typedef void (Ref::*SEL_SCHEDULE)(float);
typedef void (Ref::*SEL_CallFunc)();
typedef void (Ref::*SEL_CallFuncN)(Node*);
typedef void (Ref::*SEL_CallFuncND)(Node*, void*);
typedef void (Ref::*SEL_CallFuncO)(Ref*);
typedef void (Ref::*SEL_MenuHandler)(Ref*);
typedef int (Ref::*SEL_Compare)(Ref*);

#define schedule_selector(_SELECTOR) static_cast<cocos2d::SEL_SCHEDULE>(&_SELECTOR)
#define callfunc_selector(_SELECTOR) static_cast<cocos2d::SEL_CallFunc>(&_SELECTOR)
#define callfuncN_selector(_SELECTOR) static_cast<cocos2d::SEL_CallFuncN>(&_SELECTOR)
#define callfuncND_selector(_SELECTOR) static_cast<cocos2d::SEL_CallFuncND>(&_SELECTOR)
#define callfuncO_selector(_SELECTOR) static_cast<cocos2d::SEL_CallFuncO>(&_SELECTOR)
#define menu_selector(_SELECTOR) static_cast<cocos2d::SEL_MenuHandler>(&_SELECTOR)
#define event_selector(_SELECTOR) static_cast<cocos2d::SEL_EventHandler>(&_SELECTOR)
#define compare_selector(_SELECTOR) static_cast<cocos2d::SEL_Compare>(&_SELECTOR)

// new callbacks based on C++11
#define CC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ##__VA_ARGS__)

// end of base_nodes group
/// @}

NS_CC_END

#endif // __CCREF_H__
