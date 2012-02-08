//
//  EPNotificationCenter.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/8/30.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "EPNotificationCenter.h"

using namespace cocos2d;

static EPNotificationCenter *s_sharedNotifCenter = NULL;

EPNotificationCenter::EPNotificationCenter()
{
    m_observers = cocos2d::CCArray::arrayWithCapacity(3);
    m_observers->retain();
}

EPNotificationCenter::~EPNotificationCenter()
{
    m_observers->release();
}

EPNotificationCenter *EPNotificationCenter::sharedNotifCenter(void)
{
    if (!s_sharedNotifCenter)
    {
        s_sharedNotifCenter = new EPNotificationCenter;
    }
    return s_sharedNotifCenter;
}

//
// internal functions
//
bool EPNotificationCenter::observerExisted(CCObject *target,const char *name)
{
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        EPNotificationObserver* observer = (EPNotificationObserver*) obj;
        if (!observer)
            continue;
        
        if (!strcmp(observer->getName(),name) && observer->getTarget() == target)
            return true;
    }
    return false;
}

//
// observer functions
//
void EPNotificationCenter::addObserver(CCObject *target, 
                                       SEL_CallFuncO selector,
                                       const char *name,
                                       CCObject *obj)
{
    if (this->observerExisted(target, name))
        return;
    
    EPNotificationObserver *observer = new EPNotificationObserver(target, selector, name, obj);
    if (!observer)
        return;
    
    observer->autorelease();
    m_observers->addObject(observer);
}

void EPNotificationCenter::removeObserver(CCObject *target,const char *name)
{
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        EPNotificationObserver* observer = (EPNotificationObserver*) obj;
        if (!observer)
            continue;
        
        if (!strcmp(observer->getName(),name) && observer->getTarget() == target)
        {
            m_observers->removeObject(observer);
            return;
        }
    }
}

void EPNotificationCenter::postNotification(const char *name, CCObject *object)
{
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        EPNotificationObserver* observer = (EPNotificationObserver*) obj;
        if (!observer)
            continue;
        
        if (!strcmp(name,observer->getName()))
            observer->performSelector(object);
    }
}

void EPNotificationCenter::postNotification(const char *name)
{
    this->postNotification(name,NULL);
}

////////////////////////////////////////////////////////////////////////////////
///
/// EPNotificationObserver
///
////////////////////////////////////////////////////////////////////////////////
EPNotificationObserver::EPNotificationObserver(CCObject *target, 
                                               SEL_CallFuncO selector,
                                               const char *name,
                                               CCObject *obj)
{
    m_target = target;
    m_selector = selector;
    m_object = obj;
    
    m_name = new char[strlen(name)+1];
    memset(m_name,0,strlen(name)+1);
    
    string orig (name);
    orig.copy(m_name,strlen(name),0);
}

EPNotificationObserver::~EPNotificationObserver()
{
    if (m_name)
        delete m_name;
}

void EPNotificationObserver::performSelector(CCObject *obj)
{
    if (m_target)
    {
        (m_target->*m_selector)(obj);
    }
}

CCObject *EPNotificationObserver::getTarget()
{
    return m_target;
}

SEL_CallFuncO EPNotificationObserver::getSelector()
{
    return m_selector;
}

char *EPNotificationObserver::getName()
{
    return m_name;
}

CCObject *EPNotificationObserver::getObject()
{
    return m_object;
}