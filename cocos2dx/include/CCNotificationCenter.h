#ifndef __CCNOTIFICATIONCENTER_H__
#define __CCNOTIFICATIONCENTER_H__

#include <map>
#include <vector>
#include <string>
#include "CCObject.h"
#include "CCString.h"


namespace   cocos2d {

class CC_DLL CCNotification : public CCObject
{
public:

	CCNotification(CCString* n,CCObject* o,CCObject* ui)
	{
		myName=n;
		if(n)
		{
			n->retain();
		}

		myObject=o;
		if(o)
		{
			o->retain();
		}
		myUserInfo=ui;
		if(ui)
		{
			ui->retain();
		}
	}
	~CCNotification()
	{
		if(myName)
		{
			myName->release();
		}
		myName=0;
		
		if(myObject)
		{
			myObject->release();
		}
		myObject=0;
		
		if(myUserInfo)
		{
			myUserInfo->release();
		}
		myUserInfo=0;
	};

	CCString*	name()
	{
		return myName;
	}
	CCObject*	object()
	{
		return myObject;
	}
	CCObject*	userInfo()
	{
		return myUserInfo;
	}


protected:
	CCString*	myName;
	CCObject*	myObject;
	CCObject*	myUserInfo;
};

class CC_DLL CCNotificationCenter : public CCObject
{
public:
	~CCNotificationCenter();
	CCNotificationCenter();
	
	static CCNotificationCenter* defaultCenter()
	{
		if(myDefaultCenter==0)
		{
			myDefaultCenter=new CCNotificationCenter();
		}
		return myDefaultCenter;
	}

	void	update();
	void	addObserver(void* observer,SEL_CallFuncNT selector,CCString* notificationName,CCObject* sender=0);
	void	removeObserver(void* observer,CCString* notificationName=0,CCObject* sender=0);

	void	postNotificationName(CCString* notificationName,CCObject* sender=0,CCObject* userInfo=0);
protected:

	int		postLevel;
	
	static CCNotificationCenter* myDefaultCenter;

	struct ObserverStruct
	{
		bool				myIsRemoved;
		SEL_CallFuncNT		mySelector;
		CCObject*			mySender;
	};

	struct ObserverStructVector
	{
		void*						myObserver;
		std::vector<ObserverStruct>	myVector;
	};

	struct NotifVectorStruct
	{
		std::vector<std::string>	myVector;
	};

	// map associating observer to notifications...
	std::map<void*, NotifVectorStruct>	myObserverMap;

	// map associating notification name to observers map
	std::map<std::string,std::vector<ObserverStructVector> >		myNotificationMap;

	void protectedRemoveObserver(void* observer,const std::string&  notificationName,CCObject* sender);
	void protectedRemoveObserver(void* observer);
	void protectedSetRemoveState(void* observer,const std::string&  notificationName,CCObject* sender);
	void protectedSetRemoveState(void* observer);

	void manageRemoveObserverList();

	struct removeObserverStruct
	{
		std::string notificationName;
		CCObject* sender;
	};

	std::map<void*,std::vector<removeObserverStruct> >	myRemoveObserverMap;

};
}//namespace   cocos2d 
#endif //__CCNOTIFICATIONCENTER_H__