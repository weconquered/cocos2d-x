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

#ifndef __COCOA_NSOBJECT_H__
#define __COCOA_NSOBJECT_H__

#include "CCCommon.h"
#include "ccTypes.h"
#include <string>
#include <map>
namespace   cocos2d {

class CCZone;
class CCObject;
class CCString;
class CCNode;
class CCEvent;
class CCNotification;

enum {
    CCOrderedAscending = -1,
    CCOrderedSame,
    CCOrderedDescending
};
typedef int CCComparisonResult;

typedef CCComparisonResult (*CCComparator)(CCObject* obj1, CCObject* obj2);
typedef CCComparisonResult (*CCComparatorWithContext)(CCObject* obj1, CCObject* obj2,CCObject* ctx);
typedef CCComparisonResult (CCObject::*SEL_Comparator)(CCObject*);

#define callcompO_selector(_SELECTOR) (SEL_Comparator)(&_SELECTOR)

extern CC_DLL CCComparatorWithContext	myCurrentComparator;
extern CC_DLL SEL_Comparator			myCurrentComparatorSelector;
extern CC_DLL CCComparator				myCurrentComparatorNoContext;
extern CC_DLL CCObject*					myCurrentComparisonContext;


// FACTORY MANAGEMENT

typedef CCObject* (*ClassCreator)();

class CC_DLL CCClassFromStringFactory
{
public:
    ~CCClassFromStringFactory();
    CCClassFromStringFactory();

    void addClass(const char*,ClassCreator);

    CCObject*	createInstance(const std::string& className);

protected:


    std::map<std::string,ClassCreator>	myCreatorMap;

};

CC_DLL extern	CCClassFromStringFactory	globalCCClassFactory;	

class DeclareToFactory
{
public:
    DeclareToFactory(const char* className,ClassCreator creator)
    {
        globalCCClassFactory.addClass(className,creator);
    }
};


inline	CCObject *	CCClassFromString(const std::string& className)
{
    return globalCCClassFactory.createInstance(className);
}

#define DECLARE_KINDOF(name,parent) \
    virtual const char*	getClassType(){return #name;}	\
    virtual bool isKindOfClass(const char* str) { if(strcmp(str,#name) == 0 )return true; return parent::isKindOfClass(str);}

#define DEFINE_TO_FACTORY(classname) \
    static CCObject*	createInstance(){return ((CCObject*)new classname());}

#define DECLARE_TO_FACTORY(classname) \
    DeclareToFactory tmp##classname(#classname,&classname::createInstance);
    
class CC_DLL CCCopying
{
public:
    virtual CCObject* copyWithZone(CCZone* pZone);
};

class CC_DLL CCObject : public CCCopying
{
protected:
	// object id
    unsigned int		m_uID;
	// count of refrence
	unsigned int		m_uReference;
	// is the object autoreleased
	bool		m_bManaged;		
public:
	CCObject(void);
	virtual ~CCObject(void);
    
    void release(void);
	void retain(void);
	CCObject* autorelease(void);
	CCObject* copy(void);
	bool isSingleRefrence(void);
	unsigned int retainCount(void);
	virtual bool isEqual(const CCObject* pObject);
    
    virtual void update(ccTime dt) {CC_UNUSED_PARAM(dt);};
    virtual const char*	getClassType(){return "CCObject";}	
    virtual bool isKindOfClass(const char* str) { if(strcmp(str,"CCObject") == 0 )return true; return false;}
	friend class CCAutoreleasePool;
};


typedef void (CCObject::*SEL_SCHEDULE)(ccTime);
typedef void (CCObject::*SEL_CallFunc)();
typedef void (CCObject::*SEL_CallFuncN)(CCNode*);
typedef void (CCObject::*SEL_CallFuncND)(CCNode*, void*);
typedef void (CCObject::*SEL_CallFuncO)(CCObject*);
typedef void (CCObject::*SEL_MenuHandler)(CCObject*);
typedef void (CCObject::*SEL_EventHandler)(CCEvent*);
typedef void (CCObject::*SEL_CallFuncNT)(CCNotification*);

#define schedule_selector(_SELECTOR) (SEL_SCHEDULE)(&_SELECTOR)
#define callfunc_selector(_SELECTOR) (SEL_CallFunc)(&_SELECTOR)
#define callfuncN_selector(_SELECTOR) (SEL_CallFuncN)(&_SELECTOR)
#define callfuncND_selector(_SELECTOR) (SEL_CallFuncND)(&_SELECTOR)
#define callfuncO_selector(_SELECTOR) (SEL_CallFuncO)(&_SELECTOR)
#define menu_selector(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)
#define event_selector(_SELECTOR) (SEL_EventHandler)(&_SELECTOR)
#define callfuncNT_selector(_SELECTOR) (SEL_CallFuncNT)(&_SELECTOR)

}//namespace   cocos2d 

#endif // __COCOA_NSOBJECT_H__
