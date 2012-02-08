//
//  EPResourceManager.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/8/30.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "EPResourceManager.h"
#include "CCArray.h"
#include "CCFileUtils.h"
#include "EPNotificationCenter.h"
//cjh #include <sys/time.h>

#define STR_HAS_PREFIX(s,p) (!strncmp( s, p, ::strlen(p)))

using namespace cocos2d;

const char *kResourceLoadedNotif = "ResourceLoadedNotif";
const char *kResourceLoadedTimeoutNotif = "ResourceLoadedTimeoutNotif";

enum{
    kEPUnitStateUnloaded = 0,
    kEPUnitStateLoaded,
    kEPUnitStateLoading
};
typedef int kEPUnitStatus;

EPResourceManager::EPResourceManager()
{
    m_resources = cocos2d::CCArray::arrayWithCapacity(4);
    m_resources->retain();
    
    m_asyncLoadCount = 0;
    m_asyncTotalCount = 0;
}

EPResourceManager::~EPResourceManager()
{
    CC_SAFE_FREE(m_resources);
}

EPResourceManager *EPResourceManager::object(void)
{
    EPResourceManager *resourceManager = new EPResourceManager;
    resourceManager->autorelease();
    return resourceManager;
}
//
// internal functions
//
long long EPResourceManager::getTime()
{
    timeval t;
    gettimeofday(&t, 0);
    return t.tv_sec * 1000000ULL + t.tv_usec;
}

bool EPResourceManager::plistExisted(const char *plist)
{
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_resources, obj)
    {
        EPResourceManagerUnit* unit = (EPResourceManagerUnit*) obj;
        if (unit)
        {
            if (!strcmp(unit->getFilename().c_str(), plist))
                return true;
        }
    }
    
    return false;
}

void EPResourceManager::addUnit(const char *pszFile, int group, bool plist, bool pvr)
{
    EPResourceManagerUnit *unit = new EPResourceManagerUnit(pszFile,group,plist,pvr);
    unit->autorelease();
    m_resources->addObject(unit);
}

void EPResourceManager::textureLoadedCallback(CCObject* obj)
{
    CCTexture2D *tex = (CCTexture2D *)obj;
    
    EPResourceManagerUnit *unit = NULL;
    CCObject* o = NULL;
    CCARRAY_FOREACH(m_resources, o)
    {
        EPResourceManagerUnit* u = (EPResourceManagerUnit*) o;
        if (u && u->getState() == kEPUnitStateLoading)
        {
            unit = u;
            break;
        }
    }
    
    if (!unit)
    {
        CCLOG("[EPResourceManager] unit not found!");
        return;
    }
    
    if (unit->getPvr())
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(unit->getPlistPath().c_str(),tex);
        CCLOG("[EPResourceManager] : addPvrResource : (%d)%s", unit->getGroup(),unit->getTexPath().c_str());
    }
    else if (unit->getPlist())
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(unit->getPlistPath().c_str(), tex);
        CCLOG("[EPResourceManager] : addPlistPngResource : (%d)%s", unit->getGroup(),unit->getTexPath().c_str());
    }
    else
    {
        CCRect rect = CCRectZero;
        rect.size = tex->getContentSize();
        CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(tex, rect);
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame, unit->getTexPath().c_str());
        
        CCLOG("[EPResourceManager] : addPngResource : (%d)%s", unit->getGroup(),unit->getTexPath().c_str());
    }
    
    unit->setState(kEPUnitStateLoaded, tex);

    m_asyncLoadCount++;
    if (m_asyncLoadCount == m_asyncTotalCount)
    {
        m_asyncTotalCount = 0;
        EPNotificationCenter::sharedNotifCenter()->postNotification(kResourceLoadedNotif);
    }
    else
    {
        CCARRAY_FOREACH(m_resources, obj)
        {
            EPResourceManagerUnit* u = (EPResourceManagerUnit*) obj;
            if (u->getState() != kEPUnitStateUnloaded)
                continue;
            
            CCLOG("[EPResourceManager] loadResourceAsync : (%d)%s",u->getGroup(),u->getTexPath().c_str());
            
            u->setState(kEPUnitStateLoading, NULL);
            CCTextureCache::sharedTextureCache()->addImageAsync(u->getTexPath().c_str(), 
                                                                this, 
                                                                callfuncO_selector(EPResourceManager::textureLoadedCallback));
            break;
        }
    }
}

void EPResourceManager::timeoutSelector(cocos2d::ccTime dt)
{
    //timeout
    m_asyncLoadCount = 0;
    m_asyncTotalCount = 0;

    CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(EPResourceManager::timeoutSelector),this);
    EPNotificationCenter::sharedNotifCenter()->postNotification(kResourceLoadedTimeoutNotif);
}

//
// load/unload resources
//
bool EPResourceManager::addPngResource(const char *pszFileName, bool plist, int group)
{
    string pszPath = pszFileName;
    pszPath = pszPath.append(".png");

    if (this->plistExisted(pszFileName))
        return false;
    
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(pszPath.c_str());
    if (!texture)
        return false;

    EPResourceManagerUnit *unit = new EPResourceManagerUnit(pszFileName,group,plist,false);
    unit->autorelease();
    m_resources->addObject(unit);

    if (plist)
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(unit->getPlistPath().c_str(), texture);
    }
    else
    {
        CCRect rect = CCRectZero;
        rect.size = texture->getContentSize();
        CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(texture, rect);
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame, unit->getTexPath().c_str());
    }
    
    unit->setState(kEPUnitStateLoaded, texture);
    
    CCLOG("[EPResourceManager] : addPngResource : (%d)%s", group,pszFileName);
    return true;
}

bool EPResourceManager::addPvrResource(const char *pszFileName, int group)
{
    string texturePath = pszFileName;
    texturePath = texturePath.append(".pvr.ccz");
    
    if (this->plistExisted(pszFileName))
        return false;
    
    string plistPath = pszFileName;
    plistPath = plistPath.append(".plist");
        
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(texturePath.c_str());
    if (!texture)
        return false;
    
    EPResourceManagerUnit *unit = new EPResourceManagerUnit(pszFileName,group,true,true);
    unit->autorelease();
    m_resources->addObject(unit);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistPath.c_str(),texture);
    
    unit->setState(kEPUnitStateLoaded, texture);
    
    CCLOG("[EPResourceManager] : addPvrResource : (%d)%s.pvr.ccz", pszFileName,pszFileName);
    return true;
}

//
// load/unload resources, ansyc
//
bool EPResourceManager::addPngResourceAsync(const char *pszFileName, bool plist, int group)
{    
    string pszPath = pszFileName;
    pszPath = pszPath.append(".png");

    if (this->plistExisted(pszFileName))
        return false;
 
    this->addUnit(pszFileName, group, plist, false);
    return true;
}

void EPResourceManager::loadResourceAsync(void)
{
    if (m_asyncTotalCount > 0)
        return;
    
    m_asyncLoadCount = 0;
    m_asyncTotalCount = 0;
    
    // calculate the total 
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_resources, obj)
    {
        EPResourceManagerUnit* unit = (EPResourceManagerUnit*) obj;
        if (unit && unit->getState() == kEPUnitStateUnloaded)
        {
            m_asyncTotalCount++;
        }
    }
    
    if (m_asyncTotalCount == 0)
    {
        EPNotificationCenter::sharedNotifCenter()->postNotification(kResourceLoadedNotif);
        return;
    }
  
    m_loadingTime = getTime();
    CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(EPResourceManager::timeoutSelector),
                                                     this, 
                                                     3.0f,
                                                     false);
    CCARRAY_FOREACH(m_resources, obj)
    {
        EPResourceManagerUnit* unit = (EPResourceManagerUnit*) obj;
        if (unit->getState() != kEPUnitStateUnloaded)
            continue;
    
        CCLOG("[EPResourceManager] loadResourceAsync : %s, group : %d",unit->getTexPath().c_str(),unit->getGroup());
        
        unit->setState(kEPUnitStateLoading, NULL);
        CCTextureCache::sharedTextureCache()->addImageAsync(unit->getTexPath().c_str(), 
                                                            this, 
                                                            callfuncO_selector(EPResourceManager::textureLoadedCallback));
        break;
    }
}

void EPResourceManager::unloadResource(int group)
{
    cocos2d::CCArray *removeList = cocos2d::CCArray::arrayWithCapacity(4);
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_resources, obj)
    {
        EPResourceManagerUnit* unit = (EPResourceManagerUnit*) obj;
        if (unit->getGroup() == group || group == -1)
        {
            unit->releaseResource(group);
            removeList->addObject(unit);
        }
    }
    
    CCARRAY_FOREACH(removeList, obj)
    {
        m_resources->removeObject(obj);
    }
}

void EPResourceManager::unloadAllResource(void)
{
    this->unloadResource(-1);
}

//
// common
//
CCSpriteFrame *EPResourceManager::spriteFrame(const char *frameName)
{
    CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    return frameCache->spriteFrameByName(frameName);
}

CCSpriteFrame *EPResourceManager::spriteFrameArg1Str(const char *format,const char *frameName)
{
    char str[128] = {0};
    sprintf(str, format,frameName);
    return this->spriteFrame(str);
}

CCSpriteFrame *EPResourceManager::spriteFrameArg1Int(const char *format,int index)
{
    char str[128] = {0};
    sprintf(str, format,index);
    return this->spriteFrame(str);
}

////////////////////////////////////////////////////////////////////////////////////
///
/// EPResourceManagerUnit implementation
///
////////////////////////////////////////////////////////////////////////////////////
EPResourceManagerUnit::EPResourceManagerUnit(const char *pszFilename, 
                                             int group, 
                                             bool plist,
                                             bool pvr)
{
    m_pszFilename = pszFilename;
    m_group = group;
    m_state = kEPUnitStateUnloaded;
    m_plist = plist;
    m_pvr = pvr;
    m_tex = NULL;

    if (m_pvr)
    {
        //pvr.ccz
        std::string basicPath = pszFilename;
        m_texPath = basicPath.append(".pvr.ccz");
        
        basicPath = pszFilename;
        m_plistPath = basicPath.append(".plist");
    }
    else if (m_plist)
    {
        //pvr.ccz
        std::string basicPath = pszFilename;
        m_texPath = basicPath.append(".png");

        basicPath = pszFilename;
        m_plistPath = basicPath.append(".plist");        
    }
    else
    {
        //png
        std::string basicPath = pszFilename;
        m_texPath = basicPath.append(".png");
        
        m_plistPath = "";
    }
}

EPResourceManagerUnit::~EPResourceManagerUnit()
{
}

int EPResourceManagerUnit::getState()
{
    return m_state;
}

void EPResourceManagerUnit::setState(int state,cocos2d::CCTexture2D *tex)
{
    m_state = state;
    m_tex = tex;
}

void EPResourceManagerUnit::releaseResource(int group)
{
    if (group != -1 && m_group != group)
        return;
    
    if (m_state != kEPUnitStateLoaded)
        return;
    
    CCLOG("[EPResourceManager] unloadResource : (%d)%s",m_group,m_pszFilename.c_str());
    
    if (m_plist)
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(m_plistPath.c_str());
    else
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(m_texPath.c_str());
    
    if (m_tex)
        CCTextureCache::sharedTextureCache()->removeTexture(m_tex);
}
